package org.vpinball;

import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.util.HashMap;
import java.util.LinkedHashMap;

import org.vpinball.Param.ParamType;

public class IDLParserToC {

    public enum ParseMode {
        NONE,
        METHODS,
    }

    public void parse(String in, String out, HashMap<String,String> interfaceMap) throws Exception {
        LinkedHashMap<String, Dispatch> dispatchMap = new LinkedHashMap<String, Dispatch>();

        BufferedReader bufferedReader = new BufferedReader(new FileReader(in));

        FileOutputStream outputStream = new FileOutputStream(out);

        int lineNo = 0;

        ParseMode mode = ParseMode.NONE;
        String className = "";

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
                        className = interfaceMap.get(interfaceName);
                        dispatchMap.clear();
                    }
                }
            }

            if (mode == ParseMode.NONE) {
                line = bufferedReader.readLine();

                continue;
            }
            else if (line.startsWith("}")) {
                if (mode == ParseMode.METHODS) {
                    outputStream.write(generateDispatch(className, dispatchMap, interfaceMap).getBytes());
                }

                mode = ParseMode.NONE;
            }
            else if (mode == ParseMode.METHODS) {
                if (!foundMethod && line.startsWith("[")) {
                    foundMethod = true;
                    methodLineNo = lineNo;
                }

                if (foundMethod) {
                    methodBuffer += line;

                    if (line.endsWith(";")) {
                        Method method = new Method(methodBuffer, methodLineNo);

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

                        foundMethod = false;
                        methodBuffer = "";
                    }
                }
            }

            line = bufferedReader.readLine();
        }

        bufferedReader.close();

        outputStream.close();
    }

    private String generateDispatch(String className, LinkedHashMap<String, Dispatch> dispatchMap, HashMap<String, String> interfaceMap) throws Exception {
        StringBuffer buffer = new StringBuffer();

        String interfaceName = null;

        for (String key : interfaceMap.keySet()) {
            if (interfaceMap.get(key).equals(className)) {
                interfaceName = key;
                break;
            }
        }

        buffer.append("static HRESULT WINAPI " + className + "_GetIDsOfNames(" + interfaceName + " *iface, REFIID riid, LPOLESTR *rgszNames,\n");
        buffer.append("                UINT cNames, LCID lcid, DISPID *rgDispId)\n");
        buffer.append("{\n"
                + "#ifndef __STANDALONE__\n"
                + "    ITypeInfo *typeinfo;\n"
                + "    HRESULT hr;\n"
                + "\n"
                + "    TRACE(\"%p, %s, %p, %u, %lx, %p.\\n\", iface, debugstr_guid(riid), rgszNames, cNames, lcid, rgDispId);\n"
                + "\n"
                + "    hr = get_typeinfo(" + interfaceName + "_tid, &typeinfo);\n"
                + "    if(SUCCEEDED(hr))\n"
                + "    {\n"
                + "        hr = ITypeInfo_GetIDsOfNames(typeinfo, rgszNames, cNames, rgDispId);\n"
                + "        ITypeInfo_Release(typeinfo);\n"
                + "    }\n"
                + "\n"
                + "    return hr;\n"
                + "#else\n");

        StringBuffer fragment = new StringBuffer();

        fragment.append("*rgDispId = DISPID_UNKNOWN;\n");
        fragment.append("\n");

        int index = 0;

        for (String key : dispatchMap.keySet()) {
            Dispatch dispatch = dispatchMap.get(key);

            if ("DISPID_NEWENUM".equals(dispatch.getId()) || 
                    "DISPID_VALUE".equals(dispatch.getId())) {
                continue;
            }

            if (index > 0) {
                fragment.append("else ");
            }
            else {
                fragment.append("");
            }

            fragment.append("if (!wcsicmp(*rgszNames, L\"" + key + "\")) {\n");
            fragment.append("*rgDispId = " + dispatch.getId() + ";\n");
            fragment.append("}\n");

            index++;
        }

        fragment.append("\n");
        fragment.append("return (*rgDispId != DISPID_UNKNOWN) ? S_OK : DISP_E_UNKNOWNNAME;\n");

        buffer.append(indent(fragment.toString(), 1));
        buffer.append("#endif\n");
        buffer.append("}\n\n");

        buffer.append("static HRESULT WINAPI " + className + "_Invoke(" + interfaceName + " *iface, DISPID dispIdMember,\n"
                + "                                      REFIID riid, LCID lcid, WORD wFlags,\n"
                + "                                      DISPPARAMS *pDispParams, VARIANT *pVarResult,\n"
                + "                                      EXCEPINFO *pExcepInfo, UINT *puArgErr)\n"
                + "{\n"
                + "    ITypeInfo *typeinfo;\n"
                + "    HRESULT hr;\n"
                + "\n"
                + "    TRACE(\"%p, %ld, %s, %lx, %d, %p, %p, %p, %p.\\n\", iface, dispIdMember, debugstr_guid(riid),\n"
                + "           lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);\n"
                + "\n"
                + "#ifndef __STANDALONE__\n"
                + "    hr = get_typeinfo(" + interfaceName + "_tid, &typeinfo);\n"
                + "    if(SUCCEEDED(hr))\n"
                + "    {\n"
                + "        hr = ITypeInfo_Invoke(typeinfo, iface, dispIdMember, wFlags,\n"
                + "                pDispParams, pVarResult, pExcepInfo, puArgErr);\n"
                + "        ITypeInfo_Release(typeinfo);\n"
                + "    }\n"
                + "\n"
                + "    return S_OK;\n"
                + "#else\n");

        fragment.setLength(0);

        fragment.append("hr = DISP_E_UNKNOWNNAME;\n");
        fragment.append("int index = pDispParams->cArgs;\n");
        fragment.append("switch(dispIdMember) {\n");

        for (String key : dispatchMap.keySet()) {
            Dispatch dispatch = dispatchMap.get(key);

            fragment.append("case " + dispatch.getId() + ": {\n"); 

            index = 0;

            for (Method method:dispatch.getMethodList()) {
                if (index > 0) {
                    fragment.append("else ");
                }
                fragment.append(generateMethod(method, className));
                index++;
            }
            fragment.append("break;\n"); 
            fragment.append("}\n"); 
        }

        fragment.append("default:\n");
        fragment.append("break;\n");
        fragment.append("}\n");
        fragment.append("if (hr != S_OK) {\n");
        fragment.append("if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {\n");
        fragment.append("V_VT(pVarResult) = VT_DISPATCH;\n");
        fragment.append("V_DISPATCH(pVarResult) = (IDispatch*)iface;\n");
        fragment.append("return S_OK;\n");
        fragment.append("}\n");
        fragment.append("else {\n");
        fragment.append("PLOGI.printf(\"dispId=%d (0x%08x), wFlags=%d, hr=%d\", dispIdMember, dispIdMember, wFlags, hr);\n");
        fragment.append("}\n");
        fragment.append("}\n");
        fragment.append("return hr;\n");
        
        buffer.append(indent(fragment.toString(), 1));
        buffer.append("#endif\n");
        buffer.append("}\n\n");

        return buffer.toString();
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
            else if ("VARIANT_BOOL".equals(param.getType())) {
                buffer.append("V_VT(&var" + index + ") = VT_BOOL;\n");
                buffer.append("V_BOOL(&var" + index + ") = " + param.getDefaultValue() + ";\n");
            }
            else if ("BSTR".equals(param.getType())) {
                buffer.append("OLECHAR* pszDefault = L" + param.getDefaultValue() + ";\n");
                buffer.append("V_VT(&var" + index + ") = VT_BSTR;\n");
                buffer.append("V_BSTR(&var" + index + ") = SysAllocString(pszDefault);\n");
            }
            else if ("CompareMethod".equals(param.getType())
                    || "Tristate".equals(param.getType())
                    || "IOMode".equals(param.getType())
                    || "StandardStreamTypes".equals(param.getType())
                    || "FileAttribute".equals(param.getType())
                    || "SpecialFolderConst".equals(param.getType())) {
                buffer.append("V_VT(&var" + index + ") = VT_I4;\n");
                buffer.append("V_I4(&var" + index + ") = " + param.getDefaultValue() + ";\n");
            }
            else {
                buffer.append("\nUNSUPPORTED DEFAULT\n");
            }
        }
        else {
            buffer.append("V_VT(&var" + index + ") = VT_EMPTY;\n");
        }

        if (type != null) {
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
            buffer.append("VariantCopyInd(&var" + index + ", &pDispParams->rgvarg[--index]);\n");
        }

        return buffer.toString();
    }

    private String generateMethod(Method method, String className) {
        StringBuffer buffer = new StringBuffer();

        buffer.append("hr = " + className + "_" + method.getPrefix() + method.getName() + "(iface");

        int index = 0;

        StringBuffer header = new StringBuffer();

        for (Param param : method.getParamList()) {
            buffer.append(", ");
          
            if (param.getParamType() == ParamType.IN) {
                if ("int".equals(param.getType())) {
                    header.append(generateHeaderVariant(index, param, "VT_I4", method));
                    buffer.append("V_I4(&var" + index + ")");
                }
                else if ("long".equalsIgnoreCase(param.getType())) {
                    header.append(generateHeaderVariant(index, param, "VT_I4", method));
                    buffer.append("V_I4(&var" + index + ")");
                }
                else if ("float".equals(param.getType())) {
                    header.append(generateHeaderVariant(index, param, "VT_R4", method));
                    buffer.append("V_R4(&var" + index + ")");
                }
                else if ("VARIANT_BOOL".equals(param.getType())) {
                    header.append(generateHeaderVariant(index, param, "VT_BOOL", method));
                    buffer.append("V_BOOL(&var" + index + ")");
                }
                else if ("BSTR".equals(param.getType())) {
                    header.append(generateHeaderVariant(index, param, "VT_BSTR", method));
                    buffer.append("V_BSTR(&var" + index + ")");
                }
                else if ("DATE".equals(param.getType())) {
                    header.append(generateHeaderVariant(index, param, "VT_DATE", method));
                    buffer.append("V_DATE(&var" + index + ")");
                }
                else if ("VARIANT*".equals(param.getType())) { 
                    header.append(generateHeaderVariant(index, param, null, method));
                    buffer.append("&var" + index );
                }
                else if ("VARIANT".equals(param.getType())) { 
                    header.append(generateHeaderVariant(index, param, "VT_VARIANT", method));
                    buffer.append("var" + index);
                }
                else if ("OLE_COLOR".equals(param.getType())) {
                    header.append(generateHeaderVariant(index, param, "VT_UI4", method));
                    buffer.append("(OLE_COLOR)V_UI4(&var" + index + ")");
                }
                else if ("CompareMethod".equals(param.getType())
                        || "Tristate".equals(param.getType())
                        || "IOMode".equals(param.getType())
                        || "StandardStreamTypes".equals(param.getType())
                        || "FileAttribute".equals(param.getType())
                        || "SpecialFolderConst".equals(param.getType())) {
                    header.append(generateHeaderVariant(index, param, "VT_I4", method));
                    buffer.append("(" + param.getType() + ")V_I4(&var" + index + ")");
                }
                else if ("IFontDisp*".equals(param.getType())) {
                    // FIX ME
                    buffer.append("(IFontDisp*)pDispParams->rgvarg");
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
                else if ("float*".equals(param.getType())) {
                    buffer.insert(0, "V_VT(pVarResult) = VT_R4;\n");
                    buffer.append("&V_R4(pVarResult)");
                }
                else if ("long*".equalsIgnoreCase(param.getType())) {
                    buffer.insert(0, "V_VT(pVarResult) = VT_I4;\n");
                    buffer.append("(LONG*)&V_I4(pVarResult)");
                }
                else if ("VARIANT_BOOL*".equals(param.getType())) {
                    buffer.insert(0, "V_VT(pVarResult) = VT_BOOL;\n");
                    buffer.append("&V_BOOL(pVarResult)");
                }
                else if ("BSTR*".equals(param.getType())) {
                    buffer.insert(0, "V_VT(pVarResult) = VT_BSTR;\n");
                    buffer.append("&V_BSTR(pVarResult)");
                }
                else if ("DATE*".equals(param.getType())) {
                    buffer.insert(0, "V_VT(pVarResult) = VT_DATE;\n");
                    buffer.append("&V_DATE(pVarResult)");
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
                else if ("IFontDisp**".equals(param.getType())
                        || "ITable**".equals(param.getType())
                        || "IDrive**".equals(param.getType())
                        || "IDriveCollection**".equals(param.getType())
                        || "IFile**".equals(param.getType())
                        || "IFolder**".equals(param.getType())
                        || "ITextStream**".equals(param.getType())
                        || "IDispatch**".equals(param.getType())) {
                    buffer.insert(0, "V_VT(pVarResult) = VT_DISPATCH;\n");
                    buffer.append("(" + param.getType() + ")&V_DISPATCH(pVarResult)");
                }
                else if ("SAFEARRAY(VARIANT)*".equals(param.getType())) {
                    buffer.insert(0, "V_VT(pVarResult) = VT_VARIANT|VT_ARRAY;\n");
                    buffer.append("(SAFEARRAY**)&V_ARRAY(pVarResult)");
                }
                else if ("CompareMethod*".equals(param.getType())
                        || "Tristate*".equals(param.getType())
                        || "IOMode*".equals(param.getType())
                        || "StandardStreamTypes*".equals(param.getType())
                        || "FileAttribute*".equals(param.getType())
                        || "SpecialFolderConst*".equals(param.getType())) {
                    buffer.insert(0, "V_VT(pVarResult) = VT_I4;\n");
                    buffer.append("(" + param.getType() + ")&V_I4(pVarResult)");
                }
                else {
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

        for (int index = 0; index < indent; index++) {
            buffer.append("\t");
        }

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
        IDLParserToC parser = new IDLParserToC();

        parser.parse(
                "../../standalone/inc/wine/dlls/scrrun/scrrun.idl", 
                "dictionary.c",
                new HashMap<String, String>() {{     
                    put("IDictionary", "dictionary");
                }});

        parser.parse(
                "../../standalone/inc/wine/dlls/scrrun/scrrun.idl", 
                "filesystem.c",
                new HashMap<String, String>() {{     
                    put("IDriveCollection", "drivecollection");
                    put("IFolderCollection", "foldercollection");
                    put("IFolder", "folder");
                    put("IScriptEncoder", "scriptencoder");
                    put("IDrive", "drive");
                }});

        parser.parse(
                "../../standalone/inc/wine/dlls/scrrun/scrrun.idl",
                "filesys.c",
                new HashMap<String, String>() {{
                    put("IFileSystem", "filesys");
                }});

        parser.parse(
                "../../standalone/inc/wine/dlls/scrrun/scrrun.idl",
                "filesys3.c",
                new HashMap<String, String>() {{
                    put("IFileSystem3", "filesys3");
                }});
        
        parser.parse(
                "../../standalone/inc/wine/dlls/scrrun/scrrun.idl",
                "textstream.c",
                new HashMap<String, String>() {{
                    put("ITextStream", "textstream");
                }});
        
        parser.parse(
                "../../standalone/inc/wine/dlls/scrrun/scrrun.idl",
                "file.c",
                new HashMap<String, String>() {{
                    put("IFile", "file");
                }});
    }
}
