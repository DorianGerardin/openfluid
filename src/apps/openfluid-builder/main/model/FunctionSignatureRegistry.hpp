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
 \file FunctionSignatureRegistry.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __FUNCTIONSIGNATUREREGISTRY_HPP__
#define __FUNCTIONSIGNATUREREGISTRY_HPP__

#include "GeneratorSignature.hpp"

class FunctionSignatureRegistry
{
  public:
    typedef std::vector<openfluid::machine::SignatureItemInstance*>
        FctSignatures_t;
    typedef std::map<openfluid::base::ModelItemDescriptor::ModelItemType,
        FctSignatures_t> FctSignaturesByType_t;
    virtual FctSignaturesByType_t getFctSignatures() = 0;
    virtual FctSignatures_t getGeneratorSignatures() = 0;
    virtual FctSignatures_t getPluggableSignatures() = 0;
    virtual void updatePluggableSignatures() = 0;
  protected:
    virtual void addAPluggableSignature(
        openfluid::machine::SignatureItemInstance* Signature) = 0;
    virtual void addAGeneratorSignature(
        openfluid::machine::SignatureItemInstance* Signature) = 0;
};

class FunctionSignatureRegistryImpl: public FunctionSignatureRegistry
{
  private:
    FctSignaturesByType_t m_Signatures;
  protected:
    void addAPluggableSignature(
        openfluid::machine::SignatureItemInstance* Signature)
    {
      m_Signatures[openfluid::base::ModelItemDescriptor::PluggedFunction].push_back(
          Signature);
    }
    void addAGeneratorSignature(
        openfluid::machine::SignatureItemInstance* Signature)
    {
      m_Signatures[openfluid::base::ModelItemDescriptor::Generator].push_back(
          Signature);
    }
  public:
    FunctionSignatureRegistryImpl()
    {
      addAGeneratorSignature(new FixedGeneratorSignature());
      addAGeneratorSignature(new RandomGeneratorSignature());
      addAGeneratorSignature(new InterpGeneratorSignature());
    }
    FctSignaturesByType_t getFctSignatures()
    {
      return m_Signatures;
    }
    void updatePluggableSignatures()
    {
      for (unsigned int i = 0; i
          < openfluid::machine::PluginManager::getInstance()->getAvailableFunctions().size(); i++)
      {
        addAPluggableSignature(
            openfluid::machine::PluginManager::getInstance()->getAvailableFunctions()[i]);
      }
    }
    FctSignatures_t getGeneratorSignatures()
    {
      return m_Signatures[openfluid::base::ModelItemDescriptor::Generator];
    }
    FctSignatures_t getPluggableSignatures()
    {
      return m_Signatures[openfluid::base::ModelItemDescriptor::PluggedFunction];
    }
};

class FunctionSignatureRegistrySub: public FunctionSignatureRegistryImpl
{
  public:
    void addAPluggableSignature(
        openfluid::machine::SignatureItemInstance* Signature)
    {
      FunctionSignatureRegistryImpl::addAPluggableSignature(Signature);
    }
    void addAGeneratorSignature(
        openfluid::machine::SignatureItemInstance* Signature)
    {
      FunctionSignatureRegistryImpl::addAGeneratorSignature(Signature);
    }
};

#endif /* __FUNCTIONSIGNATUREREGISTRY_HPP__ */
