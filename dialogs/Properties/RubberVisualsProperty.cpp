#include "stdafx.h"
#include "Properties/RubberVisualsProperty.h"
#include <WindowsX.h>

RubberVisualsProperty::RubberVisualsProperty(VectorProtected<ISelect> *pvsel) : BasePropertyDialog(IDD_PROPRUBBER_VISUALS, pvsel)
{
    m_heightEdit.SetDialog(this);
    m_thicknessEdit.SetDialog(this);
    m_rotXEdit.SetDialog(this);
    m_rotYEdit.SetDialog(this);
    m_rotZEdit.SetDialog(this);
}

void RubberVisualsProperty::UpdateVisuals(const int dispid/*=-1*/)
{
    for (int i = 0; i < m_pvsel->Size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == NULL) || (m_pvsel->ElementAt(i)->GetItemType() != eItemRubber))
            continue;
        Rubber *const rubber = (Rubber *)m_pvsel->ElementAt(i);

        if (dispid == IDC_STATIC_RENDERING_CHECK || dispid == -1)
            PropertyDialog::SetCheckboxState(m_hStaticRenderingCheck, rubber->m_d.m_staticRendering);
        if (dispid == IDC_SHOW_IN_EDITOR_CHECK || dispid == -1)
            PropertyDialog::SetCheckboxState(m_hShowInEditorCheck, rubber->m_d.m_showInEditor);
        if (dispid == IDC_RUBBER_HEIGHT_EDIT || dispid == -1)
            PropertyDialog::SetFloatTextbox(m_heightEdit, rubber->m_d.m_height);
        if (dispid == IDC_RUBBER_THICKNESS_EDIT || dispid == -1)
            PropertyDialog::SetIntTextbox(m_thicknessEdit, rubber->m_d.m_thickness);
        if (dispid == IDC_RUBBER_ROTX_EDIT || dispid == -1)
            PropertyDialog::SetFloatTextbox(m_rotXEdit, rubber->m_d.m_rotX);
        if (dispid == IDC_RUBBER_ROTY_EDIT || dispid == -1)
            PropertyDialog::SetFloatTextbox(m_rotYEdit, rubber->m_d.m_rotY);
        if (dispid == IDC_RUBBER_ROTZ_EDIT || dispid == -1)
            PropertyDialog::SetFloatTextbox(m_rotZEdit, rubber->m_d.m_rotZ);

        UpdateBaseVisuals(rubber, &rubber->m_d, dispid);
        //only show the first element on multi-select
        break;
    }
}

void RubberVisualsProperty::UpdateProperties(const int dispid)
{
    for (int i = 0; i < m_pvsel->Size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == NULL) || (m_pvsel->ElementAt(i)->GetItemType() != eItemRubber))
            continue;
        Rubber *const rubber = (Rubber *)m_pvsel->ElementAt(i);

        switch (dispid)
        {
            case IDC_STATIC_RENDERING_CHECK:
                CHECK_UPDATE_ITEM(rubber->m_d.m_staticRendering, PropertyDialog::GetCheckboxState(m_hStaticRenderingCheck), rubber);
                break;
            case IDC_SHOW_IN_EDITOR_CHECK:
                CHECK_UPDATE_ITEM(rubber->m_d.m_showInEditor, PropertyDialog::GetCheckboxState(m_hShowInEditorCheck), rubber);
                break;
            case IDC_RUBBER_HEIGHT_EDIT:
                CHECK_UPDATE_ITEM(rubber->m_d.m_height, PropertyDialog::GetFloatTextbox(m_heightEdit), rubber);
                break;
            case IDC_RUBBER_THICKNESS_EDIT:
                CHECK_UPDATE_ITEM(rubber->m_d.m_thickness, PropertyDialog::GetIntTextbox(m_thicknessEdit), rubber);
                break;
            case IDC_RUBBER_ROTX_EDIT:
                CHECK_UPDATE_ITEM(rubber->m_d.m_rotX, PropertyDialog::GetFloatTextbox(m_rotXEdit), rubber);
                break;
            case IDC_RUBBER_ROTY_EDIT:
                CHECK_UPDATE_ITEM(rubber->m_d.m_rotY, PropertyDialog::GetFloatTextbox(m_rotYEdit), rubber);
                break;
            case IDC_RUBBER_ROTZ_EDIT:
                CHECK_UPDATE_ITEM(rubber->m_d.m_rotZ, PropertyDialog::GetFloatTextbox(m_rotZEdit), rubber);
                break;
            default:
                UpdateBaseProperties(rubber, &rubber->m_d, dispid);
                break;
        }
    }
    UpdateVisuals(dispid);
}

BOOL RubberVisualsProperty::OnInitDialog()
{
    AttachItem(DISPID_Image, m_imageCombo);
    m_baseImageCombo = &m_imageCombo;
    AttachItem(IDC_MATERIAL_COMBO, m_materialCombo);
    m_baseMaterialCombo = &m_materialCombo;
    m_hStaticRenderingCheck = ::GetDlgItem(GetHwnd(), IDC_STATIC_RENDERING_CHECK);
    m_hVisibleCheck = ::GetDlgItem(GetHwnd(), IDC_VISIBLE_CHECK);
    m_hReflectionEnabledCheck = ::GetDlgItem(GetHwnd(), IDC_REFLECT_ENABLED_CHECK);
    m_heightEdit.AttachItem(IDC_RUBBER_HEIGHT_EDIT);
    m_thicknessEdit.AttachItem(IDC_RUBBER_THICKNESS_EDIT);
    m_rotXEdit.AttachItem(IDC_RUBBER_ROTX_EDIT);
    m_rotYEdit.AttachItem(IDC_RUBBER_ROTY_EDIT);
    m_rotZEdit.AttachItem(IDC_RUBBER_ROTZ_EDIT);
    m_hShowInEditorCheck = ::GetDlgItem(GetHwnd(), IDC_SHOW_IN_EDITOR_CHECK);
    UpdateVisuals();
    return TRUE;
}
