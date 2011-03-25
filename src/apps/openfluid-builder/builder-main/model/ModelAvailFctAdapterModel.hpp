/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

 OpenFLUID is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 OpenFLUID is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file ModelAvailFctAdapterModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __MODELAVAILFCTADAPTERMODEL_HPP__
#define __MODELAVAILFCTADAPTERMODEL_HPP__

#include <sigc++/sigc++.h>

#include <gtkmm/treemodel.h>
#include <glibmm/i18n.h>

#include "FunctionSignatureRegistry.hpp"
#include "ModelAvailFctColumns.hpp"
#include "BuilderTreeStore.hpp"

class ModelAvailFctAdapterModel
{
  public:
    virtual sigc::signal<void> signal_SelectionChanged() = 0;
    virtual sigc::signal<void> signal_FunctionsChanged() = 0;
    virtual void setSignatures(
        FunctionSignatureRegistry::FctSignaturesByType_t Signatures) = 0;
    virtual Glib::RefPtr<Gtk::TreeModel> getTreeModel() = 0;
    virtual void setSelectedRow(Gtk::TreeRow Row) = 0;
    virtual openfluid::machine::SignatureItemInstance
    * getSelectedSignature() = 0;
    virtual Gtk::TreeRow getFirstAvailableRow() = 0;
};

class ModelAvailFctAdapterModelImpl: public ModelAvailFctAdapterModel
{
  private:
    sigc::signal<void> m_signal_SelectionChanged;
    sigc::signal<void> m_signal_FunctionsChanged;
    Glib::RefPtr<BuilderTreeStore> mref_TreeModel;
    ModelAvailFctColumns& m_Columns;
    Gtk::TreeRowReference* mp_PluggableTitleRowRef;
    Gtk::TreeRowReference* mp_GeneratorTitleRowRef;
    Gtk::TreeRowReference* mp_FirstAvailableRowRef;
    std::map<std::string, openfluid::machine::SignatureItemInstance*>
        m_SignaturesById;
    openfluid::machine::SignatureItemInstance* mp_SelectedSignature;
    sigc::signal<void> signal_SelectionChanged();
    sigc::signal<void> signal_FunctionsChanged();
    void createTitleRows();
    Glib::ustring replaceEmpty(Glib::ustring TextToCheck);
    void setAPluggableFunction(
        openfluid::machine::SignatureItemInstance* Function);
    void setAGeneratorFunction(
        openfluid::machine::SignatureItemInstance* Function);
    void defineFirstAvailableRow();
  public:
    ModelAvailFctAdapterModelImpl(ModelAvailFctColumns& Columns);
    ~ModelAvailFctAdapterModelImpl();
    void setSignatures(
        FunctionSignatureRegistry::FctSignaturesByType_t Signatures);
    Glib::RefPtr<Gtk::TreeModel> getTreeModel();
    void setSelectedRow(Gtk::TreeRow Row);
    openfluid::machine::SignatureItemInstance* getSelectedSignature();
    Gtk::TreeRow getFirstAvailableRow();
};
#endif /* __MODELAVAILFCTADAPTERMODEL_HPP__ */