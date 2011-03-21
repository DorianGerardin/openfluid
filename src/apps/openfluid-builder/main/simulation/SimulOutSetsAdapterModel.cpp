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
 \file SimulOutSetsAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutSetsAdapterModel.hpp"


#include "BuilderListStore.hpp"


std::string SimulOutSetsAdapterModelImpl::extractIDsString(
    openfluid::base::OutputSetDescriptor OutSetDesc)
{
  if (OutSetDesc.isAllUnits())
    return "*";

  return getStringListFromVect(OutSetDesc.getUnitsIDs());
}
std::string SimulOutSetsAdapterModelImpl::extractVariablesString(
    openfluid::base::OutputSetDescriptor OutSetDesc)
{
  if (OutSetDesc.isAllScalars() && OutSetDesc.isAllVectors())
    return "*";

  Glib::ustring StrScalar = "";
  Glib::ustring StrVector = "";

  StrScalar = getStringListFromVect(OutSetDesc.getScalars());
  StrVector = getStringListFromVect(OutSetDesc.getVectors(), true);

  Glib::ustring Sep = (StrScalar != "" && StrVector != "") ? ";" : "";

  return Glib::ustring::compose("%1%2%3", StrScalar, Sep, StrVector);

}

SimulOutSetsAdapterModelImpl::SimulOutSetsAdapterModelImpl() :
  m_SelectedRowRef(0)
{
  m_refListStore = BuilderListStore::create(m_Columns);
}
void SimulOutSetsAdapterModelImpl::setSets(std::map<std::string, std::pair<
    std::string, openfluid::base::OutputSetDescriptor> > SetsByName)
{
  m_SelectedRowRef = 0;

  m_refListStore->clear();

  for (std::map<std::string, std::pair<std::string,
      openfluid::base::OutputSetDescriptor> >::iterator it = SetsByName.begin(); it
      != SetsByName.end(); ++it)
  {
    Gtk::TreeRow Row = *(m_refListStore->append());

    Row[m_Columns.m_Name] = it->first;
    Row[m_Columns.m_FormatName] = it->second.first;

    openfluid::base::OutputSetDescriptor OutSetDesc = it->second.second;

    Row[m_Columns.m_UnitsClass] = OutSetDesc.getUnitsClass();
    Row[m_Columns.m_Precision] = OutSetDesc.getPrecision();
    Row[m_Columns.m_UnitsIDs] = extractIDsString(OutSetDesc);
    Row[m_Columns.m_Vars] = extractVariablesString(OutSetDesc);
  }
}
Glib::RefPtr<Gtk::TreeModel> SimulOutSetsAdapterModelImpl::getTreeModel()
{
  return m_refListStore;
}
void SimulOutSetsAdapterModelImpl::setSelectedSet(Gtk::TreeIter SelectedIter)
{
  if (SelectedIter)
    m_SelectedRowRef = m_refListStore->createRowRefFromIter(SelectedIter);
  else
    m_SelectedRowRef = 0;
}
std::string SimulOutSetsAdapterModelImpl::getSelectedSetName()
{
  if (m_SelectedRowRef)
    return m_refListStore->getRowFromRowRef(*m_SelectedRowRef)[m_Columns.m_Name];
  return "";
}


std::string SimulOutSetsAdapterModelSub::extractIDsString(
    openfluid::base::OutputSetDescriptor OutSetDesc)
{
  return SimulOutSetsAdapterModelImpl::extractIDsString(OutSetDesc);
}
std::string SimulOutSetsAdapterModelSub::extractVariablesString(
    openfluid::base::OutputSetDescriptor OutSetDesc)
{
  return SimulOutSetsAdapterModelImpl::extractVariablesString(OutSetDesc);
}
