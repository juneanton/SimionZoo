#include "stdafx.h"
#include "world-FAST.h"
#include "named-var-set.h"
#include "config.h"
#include "world.h"
#include "reward.h"
#include "app.h"
#include "app-rlsimion.h"
#include "../tools/WindowsUtils/Process.h"
#include <string>
#include <stdio.h>


#define FAST_CONFIG_FILE "fast-config.fst"
#define PORTAL_CONFIG_FILE "FASTDimensionalPortalDLL.xml"
#define MAX_CONFIG_FILE_SIZE 10000
#define DIMENSIONAL_PORTAL_PIPE_NAME "FASTDimensionalPortal"

CFASTWindTurbine::CFASTWindTurbine(CConfigNode* pConfigNode)
{
	METADATA("World", "FAST-Wind-turbine");

	//model constants
	addConstant("RatedPower", 5e6);				//W
	addConstant("HubHeight", 90);				//m
	addConstant("CutInWindSpeed", 3.0);			//m/s
	addConstant("RatedWindSpeed", 11.4);		//m/s
	addConstant("CutOutWindSpeed", 25.0);		//m/s
	addConstant("CutInRotorSpeed", 0.72256);	//6.9 rpm
	addConstant("CutOutRotorSpeed", 1.26711);	//12.1 rpm
	addConstant("RatedRotorSpeed", 1.26711);	//12.1 rpm
	addConstant("RatedTipSpeed", 8.377);		//80 rpm
	addConstant("RatedGeneratorSpeed", 122.91); //1173.7 rpm
	addConstant("RatedGeneratorTorque", 43093.55);
	addConstant("GearBoxRatio", 97.0);
	addConstant("ElectricalGeneratorEfficiency", 0.944); //%94.4
	addConstant("GeneratorInertia", 534116.0);			//kg*m^2
	addConstant("HubInertia", 115926.0);				//kg*m^2
	addConstant("DriveTrainTorsionalDamping", 6210000.0); //N*m/(rad/s)

	addStateVariable("T_a", "N/m", 0.0, 400000.0);
	addStateVariable("P_a", "W", 0.0, 1600000.0);
	addStateVariable("P_s", "W", 0.0, 6e6);
	addStateVariable("P_e", "W", 0.0, 6e6);
	addStateVariable("E_p", "W", -5e6,5e6);
	addStateVariable("v", "m/s", 1.0, 50.0);
	addStateVariable("omega_r", "rad/s", 0.0, 6.0);
	addStateVariable("omega_g", "rad/s", 0.0, 200.0);
	addStateVariable("E_omega_r", "rad/s", -4.0, 4.0);
	addStateVariable("d_omega_r", "rad/s^2", -2.0, 2.0);
	addStateVariable("beta", "rad", 0.0, 1.570796);
	addStateVariable("d_beta", "rad/s", -0.1396263, 0.1396263);
	addStateVariable("T_g", "N/m", 0.0, 50000.0);
	addStateVariable("d_T_g", "N/m/s", -15000, 15000);
	addStateVariable("E_int_omega_r", "rad", -100.0, 100.0);
	//action handlers
	addActionVariable("d_beta", "rad/s", -0.1396263, 0.1396263);
	addActionVariable("d_T_g", "N/m/s", -15000.0, 15000.0);



	m_pRewardFunction->addRewardComponent(new CToleranceRegionReward("E_p", 100, 1.0));
	m_pRewardFunction->initialize();

	if (CSimionApp::get())
	{
		//input/output files
#ifdef _DEBUG
		CSimionApp::get()->pSimGod->registerInputFile("../Release/FASTDimensionalPortalDLL.dll");
		CSimionApp::get()->pSimGod->registerInputFile("../Release/FAST_win32.exe");
		CSimionApp::get()->pSimGod->registerInputFile("../Release/MAP_win32.dll");
		CSimionApp::get()->pSimGod->registerInputFile("../Release/TurbSim.exe");
#else
		CSimionApp::get()->pSimGod->registerInputFile("../bin/FASTDimensionalPortalDLL.dll");
		CSimionApp::get()->pSimGod->registerInputFile("../bin/FAST_win32.exe");
		CSimionApp::get()->pSimGod->registerInputFile("../bin/MAP_win32.dll");
		CSimionApp::get()->pSimGod->registerInputFile("../bin/TurbSim.exe");
#endif
		//FAST data files
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/configFileTemplate.fst");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/90m_12mps_twr.bts");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/Cylinder1.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/Cylinder2.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/DU21_A17.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/DU25_A17.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/DU30_A17.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/DU35_A17.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/DU40_A17.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NACA64_A17.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NRELOffshrBsline5MW_AeroDyn_blade.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NRELOffshrBsline5MW_BeamDyn.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NRELOffshrBsline5MW_BeamDyn_Blade.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NRELOffshrBsline5MW_Blade.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NRELOffshrBsline5MW_InflowWind_12mps.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NRELOffshrBsline5MW_Onshore_AeroDyn15.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NRELOffshrBsline5MW_Onshore_ElastoDyn.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NRELOffshrBsline5MW_Onshore_ElastoDyn_BDoutputs.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/FAST/NRELOffshrBsline5MW_Onshore_ElastoDyn_Tower.dat");
		
		//TurbSim data files
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/TurbSimConfigTemplate.inp");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event00000.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event01917.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event02515.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event02800.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event02893.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event03167.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event03219.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event03326.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event03384.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event03613.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event03692.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event03868.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event04110.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event04434.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event04702.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Event99999.dat");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Events.les");
		CSimionApp::get()->pSimGod->registerInputFile("../config/world/TurbSim/EventData/Events.xtm");
	}
}


void CFASTWindTurbine::deferredLoadStep()
{
	std::string outConfigFileName;
	FILE *pOutConfigFile;

	//templated main config file
	char* fileContent= loadTemplateConfigFile("../config/world/FAST/configFileTemplate.fst");
	if (fileContent)
	{ 
		outConfigFileName= std::string(CSimionApp::get()->getOutputDirectory()) + std::string("/") + std::string(FAST_CONFIG_FILE);

		fopen_s(&pOutConfigFile, outConfigFileName.c_str(), "w");
		if (pOutConfigFile)
		{
			//We do know that the file has two %f that can be substituted by our values
			fprintf_s(pOutConfigFile, fileContent, CSimionApp::get()->pExperiment->getEpisodeLength()
				, CSimionApp::get()->pWorld->getDT());
			fclose(pOutConfigFile);
		}
		else CLogger::logMessage(MessageType::Error, (std::string("Couldn't create file: ") + outConfigFileName).c_str());
		delete [] fileContent;
	}
	else CLogger::logMessage(MessageType::Error,"Couldn't load config file: ../config/world/FAST/configFileTemplate.fst");

	//copy input files to experiment directory to avoid problems with FAST adding base config file's directory
	std::string commandLine;
	commandLine= std::string("copy ..\\config\\world\\FAST\\*.dat ") + std::string(CSimionApp::get()->getOutputDirectory());
	std::replace(commandLine.begin(), commandLine.end(), '/', '\\');
	system(commandLine.c_str());
	commandLine= std::string("copy ..\\config\\world\\FAST\\*.bts ") + std::string(CSimionApp::get()->getOutputDirectory());
	std::replace(commandLine.begin(), commandLine.end(), '/', '\\');
	system(commandLine.c_str());

	CLogger::logMessage(MessageType::Info, "Input files copied");

	//FASTDimensionalPortalDLL.xml -> used to pass the pipe's name to the dll
	bool pipeServerOpened= m_namedPipeServer.openUniqueNamedPipeServer(DIMENSIONAL_PORTAL_PIPE_NAME);
	if (pipeServerOpened)
	{
		outConfigFileName = std::string(CSimionApp::get()->getOutputDirectory()) + std::string("/") + std::string(PORTAL_CONFIG_FILE);
		fopen_s(&pOutConfigFile, outConfigFileName.c_str(), "w");
		if (pOutConfigFile)
		{
			fprintf_s(pOutConfigFile, "<?xml version=\"1.0\"?>\n<FAST-DIMENSIONAL-PORTAL>\n  <PIPE-NAME>%s</PIPE-NAME>\n</FAST-DIMENSIONAL-PORTAL>"
				, m_namedPipeServer.getPipeFullName());
			fclose(pOutConfigFile);
		}
		else CLogger::logMessage(MessageType::Error, (std::string("Couldn't create config file: ") + outConfigFileName).c_str());
	}
	else CLogger::logMessage(MessageType::Error, "Couldn't open named pipe server");
	CLogger::logMessage(MessageType::Info, "Connected to FASTDimensionalPortalDLL");
}

CFASTWindTurbine::~CFASTWindTurbine()
{
	m_namedPipeServer.closeServer();
	CLogger::logMessage(MessageType::Info, "Closed connection to FASTDimensionalPortalDLL");
}



void CFASTWindTurbine::reset(CState *s)
{
	//spawn the FAST exe file
	std::string commandLine;
#ifdef _DEBUG
	commandLine = std::string("../Release/fast_win32.exe");
#else
	commandLine= std::string("../bin/fast_win32.exe");
#endif
	commandLine+= std::string(" ") + std::string(CSimionApp::get()->getOutputDirectory())+ std::string("/")
		+ std::string(FAST_CONFIG_FILE);
	FASTprocess.spawn((char*)(commandLine).c_str());
	//wait for the client (FASTDimensionalPortalDLL) to connect
	m_namedPipeServer.waitForClientConnection();
	//receive(s)
	m_namedPipeServer.readToBuffer(s->getValueVector(), s->getNumVars() * sizeof(double));
}

void CFASTWindTurbine::executeAction(CState *s,const CAction *a,double dt)
{
	//send(a)
	//here we have to cheat the compiler (const). We don't want to, but we have to
	double* pActionValues = ((CAction*)a)->getValueVector();
	int numBytesToWrite = a->getNumVars() * sizeof(double);
	int numBytesWritten= m_namedPipeServer.writeBuffer(pActionValues, numBytesToWrite);
	if (numBytesToWrite != numBytesWritten)
	{
		CLogger::logMessage(MessageType::Error, "The Dimensional Portal has been remotely closed. Probably an error in FAST.");
	}

	//receive(s')
	int numBytesToRead = s->getNumVars() * sizeof(double);
	int numBytesRead= m_namedPipeServer.readToBuffer(s->getValueVector(), numBytesToRead);
	if (numBytesToRead!=numBytesWritten)
	{
		CLogger::logMessage(MessageType::Error, "The Dimensional Portal has been remotely closed. Probably an error in FAST.");
	}
}


char* CFASTWindTurbine::loadTemplateConfigFile(const char* filename)
{
	FILE *templateFile;
	int numCharsRead = 0;
	char *pBuffer= new char[MAX_CONFIG_FILE_SIZE];
	fopen_s(&templateFile, filename, "r");
	if (templateFile)
	{
		numCharsRead= fread_s(pBuffer, MAX_CONFIG_FILE_SIZE, sizeof(char), MAX_CONFIG_FILE_SIZE, templateFile);
		pBuffer[numCharsRead] = 0;
		return pBuffer;
	}
	return 0;
}