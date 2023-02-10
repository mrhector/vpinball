package org.vpinball;

import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;

import org.vpinball.Param.ParamType;

public class IDLParserToCpp {
	public enum ParseMode {
		NONE,
		METHODS,
		EVENTS,
		ENUM,
	}

	private HashMap<String, IDLEnum> getEnums(String in, List<String> enumList) throws Exception {
		LinkedHashMap<String, IDLEnum> enumMap = new LinkedHashMap<String, IDLEnum>();

		BufferedReader bufferedReader = new BufferedReader(new FileReader(in));

		ParseMode mode = ParseMode.NONE;

		StringBuffer buffer = new StringBuffer();
		buffer.append("\n");

		StringBuffer buffer2 = new StringBuffer();
		buffer2.append("\n");

		String line = bufferedReader.readLine();

		int id = 2000;

		while (line != null) {
			line = line.trim();

			if (line.startsWith("enum ")) {
				String token[] = line.split(" ");

				String enumName = token[1];

				if (enumList.contains(enumName)) {
					mode = ParseMode.ENUM;
				}
			}

			if (mode == ParseMode.NONE) {
				line = bufferedReader.readLine();

				continue;
			}
			else if (line.startsWith("}")) {
				mode = ParseMode.NONE;
			}
			else if (mode == ParseMode.ENUM) {
				line = line.replaceAll(" ", "");
				line = line.replaceAll(",", "");

				if (line.contains("=")) {
					String token[] = line.split("=");

					IDLEnum enumType = new IDLEnum();
					enumType.setName(token[0]);
					enumType.setValue(Integer.parseInt(token[1]));
					enumType.setId(id++);
					enumMap.put(enumType.getName(), enumType);

					//buffer.append("robin_hood::pair<wstring, int> { wstring(L\"" + token[0].toLowerCase() + "\"), " + id + " },\n");
					//buffer2.append("case " + id + ": {\n");
					//buffer2.append("if (wFlags & DISPATCH_PROPERTYGET) {\n");
					//buffer2.append("V_VT(pVarResult) = VT_I2;\n");
					//buffer2.append("V_I2(pVarResult) = " + token[1] + ";\n");
					//buffer2.append("hr = S_OK;\n");
					//buffer2.append("}\n");
					//buffer2.append("break;\n");
					//buffer2.append("}\n");

					//id++;
				}
			}

			line = bufferedReader.readLine();

		}

		bufferedReader.close();

		return enumMap;
	}

	public void parse(String in, String out, List<IDLInterface> interfaceList) throws Exception {
		Method.resetUnknownId();

		HashMap<String, IDLInterface> interfaceMap = new HashMap<String, IDLInterface>();
		for (IDLInterface idlClass:interfaceList) {
			interfaceMap.put(idlClass.getInterfaceName(), idlClass);
		}

		LinkedHashMap<String, Dispatch> dispatchMap = new LinkedHashMap<String, Dispatch>();
		ArrayList<Event> eventList = new ArrayList<Event>();

		BufferedReader bufferedReader = new BufferedReader(new FileReader(in));

		FileOutputStream outputStream = new FileOutputStream(out);

		outputStream.write("#include \"stdafx.h\"\n".getBytes());
		outputStream.write("#include \"olectl.h\"\n".getBytes());
		outputStream.write("\n".getBytes());
		outputStream.write("extern \"C\" int wcsicmp( LPCWSTR str1, LPCWSTR str2 );\n".getBytes());
		outputStream.write("\n".getBytes());

		int lineNo = 0;

		ParseMode mode = ParseMode.NONE;
		IDLInterface idlInterface = null;

		boolean foundMethod = false;
		int methodLineNo = 0;
		String methodBuffer = "";

		String line = bufferedReader.readLine();

		while (line != null) {
			lineNo++;

			line = line.trim();

			if (line.startsWith("interface ")) {
				if (line.contains(":")) {
					String interfaceName = line.split(":")[0];
					interfaceName = interfaceName.substring("interface ".length()).trim();

					if (interfaceMap.containsKey(interfaceName)) {
						mode = ParseMode.METHODS;
						idlInterface = interfaceMap.get(interfaceName);
						dispatchMap.clear();
					}
					else {
						System.out.println("Interface not in map: " + interfaceName);
					}
				}
			}
			else if (line.startsWith("dispinterface ")) {
				String interfaceName = line.split(" ")[1].trim();

				if (interfaceMap.containsKey(interfaceName)) {
					mode = ParseMode.EVENTS;
					idlInterface = interfaceMap.get(interfaceName);
					eventList.clear();
				}
			}

			if (mode == ParseMode.NONE) {
				line = bufferedReader.readLine();

				continue;
			}
			else if (line.startsWith("}")) {
				if (mode == ParseMode.METHODS) {
					HashMap<String, IDLEnum> enumMap = null;

					if (idlInterface.getEnumList() != null) {
						enumMap = getEnums(in, idlInterface.getEnumList());
					}

					outputStream.write(generateDispatch(idlInterface, dispatchMap, interfaceMap, enumMap).getBytes());
				}
				else if (mode == ParseMode.EVENTS) {
					outputStream.write(generateEvents(idlInterface, eventList).getBytes());
				}

				mode = ParseMode.NONE;
			}
			else if (mode == ParseMode.METHODS) {
				if (!foundMethod && line.startsWith("[")) {
					if (!line.contains("[restricted]")) {
						foundMethod = true;
						methodLineNo = lineNo;
					}
				}
				else if (!foundMethod && line.startsWith("/* ! */")) {
					foundMethod = true;
					methodLineNo = lineNo;
				}

				if (foundMethod) {
					methodBuffer += line;

					if (line.endsWith(";")) {
						Method method = new Method(methodBuffer, methodLineNo);

						boolean ignore = false;

						if ("JoltAmount".equals(method.getName()) ||
								"TiltAmount".equals(method.getName()) ||
								"JoltTriggerTime".equals(method.getName()) ||
								"TiltTriggerTime".equals(method.getName())) {
							ignore = true;
						}

						if ("GetPlayerHWnd".equals(method.getName())) {
							if ("long*".equals(method.getParamList().get(0).getType())) {
								ignore = true;
							}
						}

						if (!ignore) {
							if (dispatchMap.containsKey(method.getName())) {
								Dispatch dispatch = dispatchMap.get(method.getName());
								dispatch.addMethod(method);
							}
							else {
								Dispatch dispatch = new Dispatch();
								dispatch.setId(method.getId());
								dispatch.addMethod(method);

								dispatchMap.put(method.getName(), dispatch);
							}
						}

						foundMethod = false;
						methodBuffer = "";
					}
				}
			}
			else if (mode == ParseMode.EVENTS) {
				if (line.startsWith("[id(")) {
					eventList.add(new Event(line, lineNo));
				}
			}

			line = bufferedReader.readLine();
		}

		bufferedReader.close();

		outputStream.close();
	}

	private String generateDispatch(IDLInterface idlInterface, LinkedHashMap<String, Dispatch> dispatchMap,
			HashMap<String, IDLInterface> interfaceMap, HashMap<String, IDLEnum> enumMap) throws Exception {
		StringBuffer buffer = new StringBuffer();

		//buffer.append("robin_hood::unordered_map<wstring, int> " + className + "::m_nameIDMap = {\n");
		buffer.append("name_disp_id_t " + idlInterface.getClassName() + "::m_nameIDMap[] = {\n");
		buffer.append("{ NULL }");

		ArrayList<String> keys = new ArrayList<>(dispatchMap.keySet());

		if (enumMap != null) {
			keys.addAll(enumMap.keySet());
		}

		Collections.sort(keys, String.CASE_INSENSITIVE_ORDER);

		for (String key : keys) {
			if (dispatchMap.containsKey(key)) {
				Dispatch dispatch = dispatchMap.get(key);

				if ("DISPID_NEWENUM".equals(dispatch.getId()) || 
						"DISPID_VALUE".equals(dispatch.getId())) {
					continue;
				}

				buffer.append(",\n");

				//buffer.append("robin_hood::pair<wstring, int> { wstring(L\"" + key.toLowerCase() + "\"), " + dispatch.getId() + " }");
				buffer.append("{ L\"" + key + "\", " + dispatch.getId() + " }");
			}
			else if (enumMap != null && enumMap.containsKey(key)) {
				IDLEnum idlEnum = enumMap.get(key);

				buffer.append(",\n");

				//buffer.append("robin_hood::pair<wstring, int> { wstring(L\"" + key.toLowerCase() + "\"), " + dispatch.getId() + " }");
				buffer.append("{ L\"" + key + "\", " + idlEnum.getId() + " }");
			}
			else {
				System.out.println("\"" + key + "\" not found");
			}
		}

		buffer.append("\n");
		buffer.append("};\n");
		buffer.append("\n");
		buffer.append("STDMETHODIMP " + idlInterface.getClassName() + "::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {\n");

		//buffer.append("wstring name = wstring(*rgszNames);\n");
		//buffer.append("std::transform(name.begin(), name.end(), name.begin(), tolower);\n");
		//buffer.append("const robin_hood::unordered_map<wstring, int>::iterator it = m_nameIDMap.find(name);\n");
		//buffer.append("if (it != m_nameIDMap.end()) {\n");
		//buffer.append("*rgDispId = it->second;\n");
		//buffer.append("return S_OK;\n");
		//buffer.append("}\n");
		//buffer.append("return DISP_E_UNKNOWNNAME;\n");

		buffer.append("size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;\n");
		buffer.append("int r;\n");
		buffer.append("while(min <= max) {\n");
		buffer.append("i = (min + max) / 2;\n");
		buffer.append("r = wcsicmp(m_nameIDMap[i].name, *rgszNames);\n");
		buffer.append("if(!r) {\n");
		buffer.append("*rgDispId = m_nameIDMap[i].dispId;\n");
	    buffer.append("return S_OK;\n");
	    buffer.append("}\n");
	    buffer.append("if(r < 0)\n");
	    buffer.append("   min = i+1;\n");
	    buffer.append("else\n");
	    buffer.append("   max = i-1;\n");
	    buffer.append("}\n");
	    buffer.append("return DISP_E_MEMBERNOTFOUND;\n");

		buffer.append("}\n");
		buffer.append("\n");

		buffer.append("STDMETHODIMP " + idlInterface.getClassName() + "::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {\n");
		buffer.append("HRESULT hr = DISP_E_UNKNOWNNAME;\n");
		buffer.append("int index = pDispParams->cArgs;\n");
		buffer.append("switch(dispIdMember) {\n");

		for (String key : dispatchMap.keySet()) {
			Dispatch dispatch = dispatchMap.get(key);

			buffer.append("case " + dispatch.getId() + ": {\n"); 

			int index = 0;

			for (Method method:dispatch.getMethodList()) {
				if (index > 0) {
					buffer.append("else ");
				}
				buffer.append(generateMethod(method));
				index++;
			}
			buffer.append("break;\n"); 
			buffer.append("}\n"); 
		}

        if (enumMap != null) {
	        for (String key : enumMap.keySet()) {
	            IDLEnum idlEnum = enumMap.get(key);

	            buffer.append("case " + idlEnum.getId() + ": {\n");
				buffer.append("if (wFlags & DISPATCH_PROPERTYGET) {\n");
				buffer.append("V_VT(pVarResult) = VT_I2;\n");
				buffer.append("V_I2(pVarResult) = " + idlEnum.getValue() + ";\n");
				buffer.append("hr = S_OK;\n");
				buffer.append("}\n");
				buffer.append("break;\n");
				buffer.append("}\n");
	        }
        }

		buffer.append("default:\n");
		buffer.append("break;\n");
		buffer.append("}\n");
		buffer.append("if (hr != S_OK) {\n");
		buffer.append("if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {\n");
		buffer.append("V_VT(pVarResult) = VT_DISPATCH;\n");
		buffer.append("V_DISPATCH(pVarResult) = this;\n");
		buffer.append("return S_OK;\n");
		buffer.append("}\n");
		buffer.append("else if (hr != S_FALSE) {\n");
		buffer.append("PLOGI.printf(\"dispId=%d (0x%08x), wFlags=%d, hr=%d\", dispIdMember, dispIdMember, wFlags, hr);\n");
		buffer.append("}\n");
		buffer.append("}\n");
		buffer.append("return hr;\n");
		buffer.append("}\n");
		buffer.append("\n");

		buffer.append("STDMETHODIMP " + idlInterface.getClassName() + "::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {\n");
		buffer.append("if (index == MEMBERID_NIL) {\n");
		buffer.append("*pBstrName = SysAllocString(L\"" + idlInterface.getDocumentationName() + "\");\n");
		buffer.append("return S_OK;\n");
		buffer.append("}\n");
		buffer.append("return E_NOTIMPL;\n");
		buffer.append("}\n");
		buffer.append("\n");

		return indent(buffer.toString(), 0);
	}

	private String generateEvents(IDLInterface idlInterface, List<Event> eventList) throws Exception {
		HashMap<String, Integer> eventDispIdMap = new HashMap<String, Integer>();
		eventDispIdMap.put("DISPID_GameEvents_KeyDown", 1000);
		eventDispIdMap.put("DISPID_GameEvents_KeyUp", 1001);
		eventDispIdMap.put("DISPID_GameEvents_Init", 1002);
		eventDispIdMap.put("DISPID_GameEvents_MusicDone", 1003);
		eventDispIdMap.put("DISPID_GameEvents_Exit", 1004);
		eventDispIdMap.put("DISPID_GameEvents_Paused", 1005);
		eventDispIdMap.put("DISPID_GameEvents_UnPaused", 1006);
		eventDispIdMap.put("DISPID_SurfaceEvents_Slingshot", 1101);
		eventDispIdMap.put("DISPID_FlipperEvents_Collide", 1200);
		eventDispIdMap.put("DISPID_TimerEvents_Timer", 1300);
		eventDispIdMap.put("DISPID_SpinnerEvents_Spin", 1301);
		eventDispIdMap.put("DISPID_TargetEvents_Dropped", 1302);
		eventDispIdMap.put("DISPID_TargetEvents_Raised", 1303);
		eventDispIdMap.put("DISPID_LightSeqEvents_PlayDone", 1320);
		eventDispIdMap.put("DISPID_HitEvents_Hit", 1400);
		eventDispIdMap.put("DISPID_HitEvents_Unhit", 1401);
		eventDispIdMap.put("DISPID_LimitEvents_EOS", 1402);
		eventDispIdMap.put("DISPID_LimitEvents_BOS", 1403);
		eventDispIdMap.put("DISPID_AnimateEvents_Animate", 1404);

		StringBuffer buffer = new StringBuffer();

		//buffer.append("robin_hood::unordered_map<int, wstring> " + className + "::m_idNameMap[] = {\n");
		buffer.append("disp_id_name_t " + idlInterface.getClassName() + "::m_idNameMap[] = {\n");
		buffer.append("{ NULL }");

		HashMap<String, Event> eventMap = new HashMap<String, Event>();
		for (Event event : eventList) {
			eventMap.put(event.getId(), event);
		}

		ArrayList<String> keys = new ArrayList<>(eventMap.keySet());
		Collections.sort(keys, new Comparator<String>() {
			@Override
			public int compare(String o1, String o2) {
				int id1 = 0;
				int id2 = 0;
				if (eventDispIdMap.containsKey(o1)) {
					id1 = eventDispIdMap.get(o1);
				}
				else {
					try {
						id1 = Integer.parseInt(o1);
					}
					catch(Exception e) {
						System.out.println(o1 + " is not a sort key");
					}
				}
				if (eventDispIdMap.containsKey(o2)) {
					id2 = eventDispIdMap.get(o2);
				}
				else {
					try {
						id2 = Integer.parseInt(o2);
					}
					catch(Exception e) {
						System.out.println(o2 + " is not a sort key");
					}
				}
				return Integer.compare(id1, id2);
			}
		});

		for (String key : keys) {
			Event event = eventMap.get(key);
			buffer.append(",\n");
			buffer.append("{ " + event.getId() + ", L\"_" + event.getName() + "\" }");
		}
		buffer.append("\n");
		buffer.append("};\n");
		buffer.append("\n");

		buffer.append("HRESULT " + idlInterface.getClassName() + "::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {\n");
		buffer.append("static WCHAR wzName[MAXSTRING];\n");
		buffer.append("CComPtr<IDispatch> disp;\n");
		buffer.append("g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);\n");

		//buffer.append("const robin_hood::unordered_map<int, wstring>::iterator it = m_idNameMap.find(dispid);\n");
		//buffer.append("if (it != m_idNameMap.end()) {\n");
		//buffer.append("wstring name = wstring(m_wzName) + it->second;\n");
		//buffer.append("LPOLESTR fnNames = (LPOLESTR)name.c_str();\n");
		//buffer.append("\n");
		//buffer.append("DISPID tDispid;\n");
		//buffer.append("const HRESULT hr = disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid);\n");
		//buffer.append("\n");
		//buffer.append("if (hr == S_OK) {\n");
		//buffer.append("disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);\n");
		//buffer.append("}\n");
		//buffer.append("}\n");
		//buffer.append("\n");
		//buffer.append("return S_OK;\n");

		buffer.append("size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;\n");
		buffer.append("int r;\n");
		buffer.append("while(min <= max) {\n");
		buffer.append("i = (min + max) / 2;\n");
		buffer.append("if (m_idNameMap[i].dispId == dispid) {\n");
		buffer.append("wcscpy(wzName, m_wzName);\n");
		buffer.append("wcscat(wzName, m_idNameMap[i].name);\n");
		buffer.append("LPOLESTR fnNames = (LPOLESTR)wzName;\n");
		buffer.append("DISPID tDispid;\n");
		buffer.append("if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {\n");
		buffer.append("return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);\n");
		buffer.append("}\n");
		buffer.append("return DISP_E_MEMBERNOTFOUND;\n");
		buffer.append("}\n");
		buffer.append("else if (m_idNameMap[i].dispId < dispid)\n");
		buffer.append("   min = i+1;\n");
		buffer.append("else\n");
		buffer.append("   max = i-1;\n");
		buffer.append("}\n");
		buffer.append("return DISP_E_MEMBERNOTFOUND;\n");

		buffer.append("}\n");
		buffer.append("\n");

		return indent(buffer.toString(), 0);
	}

	private String generateHeaderVariant(int index, Param param, String type, Method method) {
		StringBuffer buffer = new StringBuffer();

		buffer.append("VARIANT var" + index + ";\n");

		if (param.hasDefaultValue()) {
			if ("int".equals(param.getType())) {
				buffer.append("V_VT(&var" + index + ") = VT_I4;\n");
				buffer.append("V_I4(&var" + index + ") = " + param.getDefaultValue() + ";\n");
			}
			else if ("long".equals(param.getType())) {
				buffer.append("V_VT(&var" + index + ") = VT_I4;\n");
				buffer.append("V_I4(&var" + index + ") = " + param.getDefaultValue() + ";\n");
			}
			else if ("float".equals(param.getType())) {
				buffer.append("V_VT(&var" + index + ") = VT_R4;\n");
				buffer.append("V_R4(&var" + index + ") = " + param.getDefaultValue() + ";\n");
			}
			else if ("single".equals(param.getType())) {
				buffer.append("V_VT(&var" + index + ") = VT_R4;\n");
				buffer.append("V_R4(&var" + index + ") = " + param.getDefaultValue() + ";\n");
			}
			else if ("double".equals(param.getType())) {
				buffer.append("V_VT(&var" + index + ") = VT_R8;\n");
				buffer.append("V_R8(&var" + index + ") = " + param.getDefaultValue() + ";\n");
			}
			else if ("VARIANT_BOOL".equals(param.getType())) {
				buffer.append("V_VT(&var" + index + ") = VT_BOOL;\n");
				buffer.append("V_BOOL(&var" + index + ") = " + param.getDefaultValue() + ";\n");
			}
			else if ("LONG_PTR".equals(param.getType())) {
				buffer.append("V_VT(&var" + index + ") = VT_UI4;\n");
				buffer.append("V_UI4(&var" + index + ") = " + param.getDefaultValue() + ";\n");
			}
			else if ("BSTR".equals(param.getType())) {
				buffer.append("OLECHAR* pszDefault = L" + param.getDefaultValue() + ";\n");
				buffer.append("V_VT(&var" + index + ") = VT_BSTR;\n");
				buffer.append("V_BSTR(&var" + index + ") = SysAllocString(pszDefault);\n");
			}
			else {
				buffer.append("\nUNSUPPORTED DEFAULT\n");
			}
		}
		else {
			buffer.append("V_VT(&var" + index + ") = VT_EMPTY;\n");
		}

		if (type != null) {
			if (!param.getType().equals("VARIANT")) {
				buffer.append("VariantChangeType(&var" + index + ", ");

				if (param.hasDefaultValue() || param.isOptional()) {
					buffer.append("(index > 0) ? &pDispParams->rgvarg[--index] : &var" + index);
				}
				else {
					buffer.append("&pDispParams->rgvarg[--index]");
				}

				buffer.append(", 0, " + type + ");\n");
			}
			else {
				buffer.append("VariantCopyInd(&var" + index + ", ");

				if (param.hasDefaultValue() || param.isOptional()) {
					buffer.append("(index > 0) ? &pDispParams->rgvarg[--index] : &var" + index);
				}
				else {
					buffer.append("&pDispParams->rgvarg[--index]");
				}

				buffer.append(");\n");
			}
		}
		else {
			buffer.append("VariantCopyInd(&var" + index + ", &pDispParams->rgvarg[--index]);\n");
		}

		return buffer.toString();
	}

	private String generateMethod(Method method) {
		StringBuffer buffer = new StringBuffer();

		buffer.append("hr = " + method.getPrefix() + method.getName() + "(");

		int index = 0;

		StringBuffer header = new StringBuffer();

		for (Param param : method.getParamList()) {
			if (index > 0) {
				buffer.append(", ");
			}

			if (param.getParamType() == ParamType.IN) {
				if ("int".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_I4", method));
					buffer.append("V_I4(&var" + index + ")");
				}
				else if ("long".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_I4", method));
					buffer.append("V_I4(&var" + index + ")");
				}
				else if ("unsigned short".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_UI2", method));
					buffer.append("V_UI2(&var" + index + ")");
				}
				else if ("float".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_R4", method));
					buffer.append("V_R4(&var" + index + ")");
				}
				else if ("single".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_R4", method));
					buffer.append("V_R4(&var" + index + ")");
				}
				else if ("double".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_R8", method));
					buffer.append("V_R8(&var" + index + ")");
				}
				else if ("VARIANT_BOOL".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_BOOL", method));
					buffer.append("V_BOOL(&var" + index + ")");
				}
				else if ("BSTR".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_BSTR", method));
					buffer.append("V_BSTR(&var" + index + ")");
				}
				else if ("VARIANT*".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, null, method));
					buffer.append("&var" + index );
				}
				else if ("VARIANT".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_VARIANT", method));
					buffer.append("var" + index);
				}
				else if ("IUnknown*".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, null, method));
					buffer.append("(IUnknown*)&var" + index );
				}
				else if ("IWMPMedia*".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, null, method));
					buffer.append("(IWMPMedia*)&var" + index );
				}
				else if ("IWMPPlaylist*".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, null, method));
					buffer.append("(IWMPPlaylist*)&var" + index );
				}
				else if ("IGroupActor*".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, null, method));
					buffer.append("(IGroupActor*)&var" + index );
				}
				else if ("_Bitmap*".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, null, method));
					buffer.append("(_Bitmap*)&var" + index );
				}
				else if ("IFontDisp*".equals(param.getType())) {
					// FIX ME
					header.append(generateHeaderVariant(index, param, null, method));
					buffer.append("(IFontDisp*)&var" + index );
				}
				else if ("OLE_COLOR".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_UI4", method));
					buffer.append("(OLE_COLOR)V_UI4(&var" + index + ")");
				}
				else if ("int**".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_INT_PTR", method));
					buffer.append("(int**)V_INT_PTR(&var" + index + ")");
				}
				else if ("LONG_PTR".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_UI4", method));
					buffer.append("(LONG_PTR)V_UI4(&var" + index + ")");
				}   
				else if ("UserDefaultOnOff".equals(param.getType())
						|| "FXAASettings".equals(param.getType())
						|| "PhysicsSet".equals(param.getType())
						|| "BackglassIndex".equals(param.getType())
						|| "ImageAlignment".equals(param.getType())
						|| "PlungerType".equals(param.getType()) 
						|| "TextAlignment".equals(param.getType()) 
						|| "TriggerShape".equals(param.getType())
						|| "LightState".equals(param.getType()) 
						|| "KickerType".equals(param.getType()) 
						|| "DecalType".equals(param.getType()) 
						|| "SizingType".equals(param.getType()) 
						|| "TargetType".equals(param.getType()) 
						|| "GateType".equals(param.getType()) 
						|| "RampType".equals(param.getType()) 
						|| "RampImageAlignment".equals(param.getType())
						|| "SequencerState".equals(param.getType())
						|| "RenderMode".equals(param.getType())
						|| "Interpolation".equals(param.getType())
						|| "Alignment".equals(param.getType())) {
					header.append(generateHeaderVariant(index, param, "VT_I4", method));
					buffer.append("(" + param.getType() + ")V_I4(&var" + index + ")");
				}
				else if ("void".equals(param.getType())) {
				}
				else {
					buffer.append(param.getType());
				}
			}
			else if (param.getParamType() == ParamType.OUT) {
				if ("VARIANT*".equals(param.getType())) {
					header.append("VARIANT* var" + index + " = &pDispParams->rgvarg[--index];\n");
					buffer.append("var" + index);
				}
				else {
					buffer.append(param.getType());
				}
			}
			else if (param.getParamType() == ParamType.OUT_RETVAL) {
				if ("int*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_I4;\n");
					buffer.append("(int*)&V_I4(pVarResult)");
				}
				else if ("unsigned short*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_UI2;\n");
					buffer.append("(unsigned short*)&V_UI2(pVarResult)");
				}
				else if ("single*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_R4;\n");
					buffer.append("&V_R4(pVarResult)");
				}
				else if ("float*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_R4;\n");
					buffer.append("&V_R4(pVarResult)");
				}
				else if ("double*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_R8;\n");
					buffer.append("(double*)&V_R8(pVarResult)");
				}
				else if ("long*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_I4;\n");
					buffer.append("(long*)&V_I4(pVarResult)");
				}
				else if ("VARIANT_BOOL*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_BOOL;\n");
					buffer.append("&V_BOOL(pVarResult)");
				}
				else if ("BSTR*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_BSTR;\n");
					buffer.append("&V_BSTR(pVarResult)");
				}
				else if ("OLE_COLOR*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_UI4;\n");
					buffer.append("&V_UI4(pVarResult)");
				}
				else if ("VARIANT*".equals(param.getType())) {
					buffer.append("pVarResult");
				}
				else if ("SIZE_T*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_UI4;\n");
					buffer.append("(SIZE_T*)&V_UI4(pVarResult)");
				}
				else if ("IUnknown**".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_UNKNOWN;\n");
					buffer.append("&V_UNKNOWN(pVarResult)");
				}
				else if ("IBall**".equals(param.getType())
						|| "IFontDisp**".equals(param.getType())
						|| "ITable**".equals(param.getType())
						|| "IRom**".equals(param.getType())
						|| "IRoms**".equals(param.getType())
						|| "IGame**".equals(param.getType())
						|| "IGames**".equals(param.getType())
						|| "IGameSettings**".equals(param.getType())
						|| "IControllerSettings**".equals(param.getType())
						|| "IWMPMedia**".equals(param.getType())
						|| "IWMPMediaCollection**".equals(param.getType())
						|| "IWMPControls**".equals(param.getType())
						|| "IWMPSettings**".equals(param.getType())
						|| "IWMPPlaylistCollection**".equals(param.getType())
						|| "IWMPNetwork**".equals(param.getType())
						|| "IWMPPlaylist**".equals(param.getType())
						|| "IWMPCdromCollection**".equals(param.getType())
						|| "IWMPClosedCaption**".equals(param.getType())
						|| "IWMPError**".equals(param.getType())
						|| "IGroupActor**".equals(param.getType())
						|| "IFrameActor**".equals(param.getType())
						|| "ILabelActor**".equals(param.getType())
						|| "IVideoActor**".equals(param.getType())
						|| "IImageActor**".equals(param.getType())
						|| "IUltraDMD**".equals(param.getType())
						|| "IActionFactory**".equals(param.getType())
						|| "ICompositeAction**".equals(param.getType())
						|| "ITweenAction**".equals(param.getType())
						|| "_Bitmap**".equals(param.getType())
						|| "IDispatch**".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_DISPATCH;\n");
					buffer.append("(" + param.getType() + ")&V_DISPATCH(pVarResult)");
				}
				else if ("SAFEARRAY(VARIANT)*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_VARIANT|VT_ARRAY;\n");
					buffer.append("(SAFEARRAY**)&V_ARRAY(pVarResult)");
				}
				else if ("UserDefaultOnOff*".equals(param.getType())
						|| "FXAASettings*".equals(param.getType())
						|| "PhysicsSet*".equals(param.getType())
						|| "BackglassIndex*".equals(param.getType())
						|| "ImageAlignment*".equals(param.getType())
						|| "PlungerType*".equals(param.getType())
						|| "TextAlignment*".equals(param.getType())
						|| "TriggerShape*".equals(param.getType())
						|| "LightState*".equals(param.getType())
						|| "KickerType*".equals(param.getType())
						|| "DecalType*".equals(param.getType())
						|| "SizingType*".equals(param.getType())
						|| "TargetType*".equals(param.getType())
						|| "GateType*".equals(param.getType())
						|| "RampType*".equals(param.getType())
						|| "RampImageAlignment*".equals(param.getType())
						|| "WMPOpenState*".equals(param.getType())
						|| "WMPPlayState*".equals(param.getType())
						|| "Interpolation*".equals(param.getType())
						|| "RenderMode*".equals(param.getType())) {
					buffer.insert(0, "V_VT(pVarResult) = VT_I4;\n");
					buffer.append("(" + param.getType() + ")&V_I4(pVarResult)");
				}
				else {
					System.out.println("Unknown Type: " + param.getType());
					buffer.append(param.getType());
				}
			}

			index++;
		}

		buffer.append(");\n");

		index = 0;

		for (Param param : method.getParamList()) {
			if (param.getParamType() != ParamType.OUT && 
					param.getParamType() != ParamType.OUT_RETVAL) {
				buffer.append("VariantClear(&var" + index + ");\n");
			}
			index++;
		}

		if (method.hasOutRetvalParam()) {
			String varResult = "";
			varResult += "if (pVarResult == NULL) {\n";
			varResult += "VARIANT valResult;\n";
			varResult += "VariantInit(&valResult);\n";
			varResult += "pVarResult = &valResult;\n";
			varResult += "}\n";

			header.insert(0, varResult);
		}

		buffer.insert(0, header.toString());

		buffer.insert(0, "// line " + method.getLineNo() + ": " + method.getLine() + "\n");

		String wrap = "if (wFlags & " + method.getDispatchType() +") {\n";
		wrap += buffer.toString();
		wrap += "}\n";

		return wrap;
	}

	private String indent(String code, int indent) throws Exception {
		StringBuffer buffer = new StringBuffer();

		boolean newLine = false;

		for (int i = 0; i < code.length(); i++) {
			char character = code.charAt(i);

			if (character == '\n') {
				newLine = true;
				buffer.append(character);
			}
			else {
				if (character == '{') {
					indent++;
				}
				else if (character == '}') {
					indent--;
				}

				if (newLine) {
					newLine = false;
					for (int index = 0; index < indent; index++) {
						buffer.append("\t");
					}
				}

				buffer.append(character);
			}
		}

		return buffer.toString();		
	}


	public static void main(String[] args) throws Exception {
		IDLParserToCpp parser = new IDLParserToCpp();

		parser.parse(
			"../../inc/pup/PinUpPlayer.idl",
			"pinupplayer_i_proxy.cpp",
			Arrays.asList(
				new IDLInterface("IPinDisplay", "PinUpPlayerPinDisplay", "PinDisplay")));

		parser.parse(
			"../../inc/flexdmd/FlexDMD.idl",
			"flexdmd_i_proxy.cpp",
			Arrays.asList(
				new IDLInterface("IFlexDMD", "FlexDMD"),
				new IDLInterface("IGroupActor", "FlexDMDGroupActor", "GroupActor"),
				new IDLInterface("IFrameActor", "FlexDMDFrameActor", "FrameActor"),
				new IDLInterface("ICompositeAction", "FlexDMDCompositeAction", "CompositeAction"),
				new IDLInterface("ITweenAction", "FlexDMDTweenAction", "TweenAction"),
				new IDLInterface("IActionFactory", "FlexDMDActionFactory", "ActionFactory"),
				new IDLInterface("IActor", "FlexDMDActor", "Actor"),
				new IDLInterface("IImageActor", "FlexDMDImageActor", "ImageActor"),
				new IDLInterface("IVideoActor", "FlexDMDVideoActor", "VideoActor"),
				new IDLInterface("ILabelActor", "FlexDMDLabelActor", "LabelActor")));

        parser.parse(
            "../../inc/vpinmame/VPinMAME.idl",
            "vpinmame_i_proxy.cpp",
			Arrays.asList(
				new IDLInterface("IController", "VPinMAMEController", "Controller"),
				new IDLInterface("IControllerSettings", "VPinMAMEControllerSettings", "ControllerSettings"),
				new IDLInterface("IGame", "VPinMAMEGame", "Game"),
				new IDLInterface("IGames", "VPinMAMEGames", "Games"),
				new IDLInterface("IGameSettings", "VPinMAMEGameSettings", "GameSettings"),
				new IDLInterface("IRom", "VPinMAMERom", "Rom"),
				new IDLInterface("IRoms", "VPinMAMERoms", "Roms")));

		parser.parse(
			"../../inc/wmp/wmp.idl",
			"wmp_i_proxy.cpp",
			Arrays.asList(
				new IDLInterface("IWMPCore", "WMPCore"),
				new IDLInterface("IWMPControls", "WMPControls"),
				new IDLInterface("IWMPSettings", "WMPSettings")));

		parser.parse(
			"../../../vpinball.idl",
			"vpinball_standalone_i_proxy.cpp",
			Arrays.asList(
				new IDLInterface("ICollection", "Collection"),
				new IDLInterface("ICollectionEvents", "Collection"),
				new IDLInterface("ITable", "PinTable", "Table"),
				new IDLInterface("ITableEvents", "PinTable"),
				new IDLInterface("ITableGlobal", "ScriptGlobalTable", "TableGlobal",
						Arrays.asList(
								"LightState",
								//"BackglassIndex",
								//"Filters",
								"ImageAlignment",
								"Shape",
								"TriggerShape",
								"RampType",
								"PlungerType",
								//"UserDefaultOnOff",
								//"FXAASettings",
								//"PhysicsSet",
								"TargetType",
								"GateType",
								"TextAlignment",
								"DecalType",
								"SequencerState",
								"SizingType",
								"KickerType",
								"RampImageAlignment")),
				new IDLInterface("IWall", "Surface", "Wall"),
				new IDLInterface("IWallEvents", "Surface"),
				new IDLInterface("IControlPoint", "DragPoint", "ControlPoint"),
				new IDLInterface("IFlipper", "Flipper"),
				new IDLInterface("IFlipperEvents", "Flipper"),
				new IDLInterface("ITimer", "Timer"),
				new IDLInterface("ITimerEvents", "Timer"),
				new IDLInterface("IPlunger", "Plunger"),
				new IDLInterface("IPlungerEvents", "Plunger"),
				new IDLInterface("ITextbox", "Textbox"),
				new IDLInterface("ITextboxEvents", "Textbox"),
				new IDLInterface("IBumper", "Bumper"),
				new IDLInterface("IBumperEvents", "Bumper"),
				new IDLInterface("ITrigger", "Trigger"),
				new IDLInterface("ITriggerEvents", "Trigger"),
				new IDLInterface("ILight", "Light"),
				new IDLInterface("ILightEvents", "Light"),
				new IDLInterface("IKicker", "Kicker"),
				new IDLInterface("IKickerEvents", "Kicker"),
				new IDLInterface("IPrimitive", "Primitive"),
				new IDLInterface("IPrimitiveEvents", "Primitive"),
				new IDLInterface("IHitTarget", "HitTarget"),
				new IDLInterface("IHitTargetEvents", "HitTarget"),
				new IDLInterface("IGate", "Gate"),
				new IDLInterface("IGateEvents", "Gate"),
				new IDLInterface("ISpinner", "Spinner"),
				new IDLInterface("ISpinnerEvents", "Spinner"),
				new IDLInterface("IRamp", "Ramp"),
				new IDLInterface("IRampEvents", "Ramp"),
				new IDLInterface("IFlasher", "Flasher"),
				new IDLInterface("IFlasherEvents", "Flasher"),
				new IDLInterface("IRubber", "Rubber"),
				new IDLInterface("IRubberEvents", "Rubber"),
				new IDLInterface("IDispReel", "DispReel"),
				new IDLInterface("IDispReelEvents", "DispReel"),
				new IDLInterface("ILightSeq", "LightSeq"),
				new IDLInterface("ILightSeqEvents", "LightSeq"),
				new IDLInterface("IVPDebug", "DebuggerModule", "VPDebug"),
				new IDLInterface("IDecal", "Decal"),
				new IDLInterface("IBall", "BallEx", "IBall")));
	}
}
