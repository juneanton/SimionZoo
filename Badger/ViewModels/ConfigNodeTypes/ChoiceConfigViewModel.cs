﻿using Caliburn.Micro;
using System.Xml;
using Simion;

namespace Badger.ViewModels
{
    public class ChoiceConfigViewModel : NestedConfigNode
    {
        //choices
        private string m_selectedChoiceName= null;
        private ConfigNodeViewModel m_selectedChoice = null;
        public ConfigNodeViewModel selectedChoice
        {
            get { return m_selectedChoice; }
            set { m_selectedChoice = value;
                NotifyOfPropertyChange(() => selectedChoice); }
        }
        public string selectedChoiceName
        {
            get { return m_selectedChoiceName; }
            set
            {
                m_selectedChoiceName = value;
                content = m_selectedChoiceName;
                NotifyOfPropertyChange(() => selectedChoiceName);

                initSelectedChoiceElement();
            }
        }

        private void initSelectedChoiceElement(XmlNode configNode=null)
        {
            foreach (XmlNode choiceElement in m_definitionNode.ChildNodes)
                if (choiceElement.Attributes[XMLConfig.nameAttribute].Value == m_selectedChoiceName)
                {
                    selectedChoice = getInstance(m_appViewModel
                        ,choiceElement, m_xPath, configNode);
                    m_children.Clear();
                    children.Add(selectedChoice);
                    break;
                }
        }

        private BindableCollection<string> m_choiceNames = new BindableCollection<string>();
        public BindableCollection<string> choiceNames { get { return m_choiceNames; } }

        
        public ChoiceConfigViewModel(AppViewModel appDefinition, XmlNode definitionNode, string parentXPath, XmlNode configNode = null)
        {
            string choiceElementName;
            commonInit(appDefinition,definitionNode, parentXPath);
            System.Console.WriteLine("loading " + name);

            if (configNode != null) configNode = configNode[name];

            foreach (XmlNode choiceElement in definitionNode.ChildNodes)
            {
                choiceElementName = choiceElement.Attributes[XMLConfig.nameAttribute].Value;
                
                m_choiceNames.Add(choiceElementName);
                
                if (configNode!=null && choiceElementName==configNode.ChildNodes[0].Name)
                {
                    m_selectedChoiceName = choiceElementName;
                    NotifyOfPropertyChange(() => selectedChoiceName);
                    initSelectedChoiceElement(configNode);
                }
            }
        }


        public override bool validate()
        {
            if (selectedChoiceName == null || selectedChoiceName == "") return false;
            foreach (XmlNode choiceElement in m_definitionNode)
                if (selectedChoiceName == choiceElement.Attributes[XMLConfig.nameAttribute].Value)
                    return true;
            return false;
        }

        public override string getXML(string leftSpace)
        {
            return leftSpace + getXMLHeader() + selectedChoice.getXML(leftSpace + "  ") + leftSpace + getXMLFooter();
        }

    }
}