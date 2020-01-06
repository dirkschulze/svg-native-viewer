/*
Copyright 2020 Adobe. All rights reserved.
This file is licensed to you under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License. You may obtain a copy
of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS
OF ANY KIND, either express or implied. See the License for the specific language
governing permissions and limitations under the License.
*/

#include "xml/XMLParser.h"

#include "msxml2.h"
#import "msxml3.dll"

namespace SVGNative
{
namespace xml
{
    class MSXMLNode final : public XMLNode {
    public:
        MSXMLNode(MSXML2::IXMLDOMElementPtr node)
            : mNode{node}
        {
        }

        ~MSXMLNode()
        {
            mNode.Release();
        }

        const char* GetName() override
        {
            return mNode->nodeName;
        }

        std::unique_ptr<XMLNode> GetFirstNode() override
        {
            return nullptr;
        }

        std::unique_ptr<XMLNode> GetNextSibling() override
        {
            return nullptr;
        }

        AttrValue GetAttrValue(const char* attrName, const char*) override
        {
            return {false, nullptr};
        }
    private:
        MSXML2::IXMLDOMElementPtr mNode;
    };

    class MSXMLDocument final : public XMLDocument {
    public:
        static std::unique_ptr<XMLDocument> CreateXMLDocument(const char* documentString)
        {
            auto newDocument = new MSXMLDocument(documentString);
            return std::unique_ptr<XMLDocument>(newDocument);
        }

        MSXMLDocument(const char* documentString)
        {
            mDocument.CreateInstance(__uuidof(MSXML2::DOMDocument30));
            mDocument->load(documentString);
        }

        ~MSXMLDocument()
        {
            mDocument.Release();
        }

        std::unique_ptr<XMLNode> GetFirstNode() override
        {
            if (!mDocument->documentElement)
                return nullptr;

            auto newNode = new MSXMLNode{mDocument->documentElement};
            return std::unique_ptr<XMLNode>(newNode);
        }
    private:
        MSXML2::IXMLDOMDocumentPtr mDocument;
    };

    std::unique_ptr<XMLDocument> XMLDocument::CreateXMLDocument(const char* documentString)
    {
        return MSXMLDocument::CreateXMLDocument(documentString);
    }
} // namespace xml
} // namespace SVGNative
