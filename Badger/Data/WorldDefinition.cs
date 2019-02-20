﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Xml;
using Herd.Files;

namespace Badger.Data
{
    public class WorldVar
    {
        string m_name;
        public string name { get { return m_name; } }
        double m_min;
        public double min { get { return m_min; } }
        double m_max;
        public double max { get { return m_max; } }
        string m_unit;
        public string unit { get { return m_unit; } }
        public WorldVar(string name,double min,double max,string unit)
        {
            m_name = name;
            m_min = min;
            m_max = max;
            m_unit = unit;
        }
        public WorldVar(XmlNode varDefinition)
        {
            m_name = varDefinition.Attributes[XMLTags.nameAttribute].Value;
            m_min = double.Parse(varDefinition.Attributes[XMLTags.minValueAttribute].Value);
            m_max = double.Parse(varDefinition.Attributes[XMLTags.maxValueAttribute].Value);
            m_unit = varDefinition.Attributes[XMLTags.unitAttribute].Value;
        }
    }
    public class StateVar : WorldVar
    {
        public StateVar(string name,double min,double max, string unit)
            : base(name,min,max,unit)
        {}
        public StateVar(XmlNode varDefinition) : base(varDefinition) { }
    }
    public class ActionVar : WorldVar
    {
        public ActionVar(string name, double min, double max, string unit)
            : base(name, min, max, unit)
        { }
        public ActionVar(XmlNode varDefinition) : base(varDefinition) { }
    }
    class WorldDefinition
    {
        ObservableCollection<StateVar> m_stateVars = new ObservableCollection<StateVar>();
        ObservableCollection<ActionVar> m_actionVars = new ObservableCollection<ActionVar>();
        ObservableCollection<string> m_constants = new ObservableCollection<string>();

        public void AddStateVar(StateVar var) { m_stateVars.Add(var); }
        public void AddActionVar(ActionVar var) { m_actionVars.Add(var); }
        public void AddConstant(string name) { m_constants.Add(name); }

        public List<string> GetStateVarNameList()
        {
            List<string> outList = new List<string>();
            foreach (StateVar var in m_stateVars) outList.Add(var.name);
            return outList;
        }
        public List<string> GetActionVarNameList()
        {
            List<string> outList = new List<string>();
            foreach (ActionVar var in m_actionVars) outList.Add(var.name);
            return outList;
        }
        public List<string> GetConstantNameList()
        {
            List<string> outList = new List<string>();
            foreach (string name in m_constants) outList.Add(name);
            return outList;
        }
    }
}
