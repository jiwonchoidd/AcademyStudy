#include <iostream>

int g_iType = 0;// sun
#define SUN 0
#define HP 1
void SunWork() {
	//Scanner()
	//Parser()
	//Generator()
	//Optimizer()
}
void HpWork() {
	//Scanner()
	//Parser()
	//Generator()
	//Optimizer()
}

class Scanner {};
class Parser {};
class Generator {};
class Optimizer {};
class HPScanner : public Scanner {};
class HPParser : public Parser {};
class HPGenerator : public Generator {};
class HPOptimizer : public Optimizer {};

class SunScanner : public Scanner {};
class SunParser : public Parser {};
class SunGenerator : public Generator {};
class SunOptimizer : public Optimizer {};

class CompilerFactory
{
public:
	virtual Scanner* CreateScanner() = 0;
	virtual Parser* CreateParser() = 0;
	virtual Generator* CreateGenerator() = 0;
	virtual Optimizer* CreateOptimizer() = 0;
};
class HPCompilerFactory : public CompilerFactory
{
public:
	Scanner * CreateScanner() { return new HPScanner; };
	Parser* CreateParser() { return new HPParser; };
	Generator* CreateGenerator() { return new HPGenerator; };
	Optimizer* CreateOptimizer() { return new HPOptimizer; };
};
class SunCompilerFactory : public CompilerFactory
{
public:
	Scanner * CreateScanner() { return new SunScanner; };
	Parser* CreateParser() { return new SunParser; };
	Generator* CreateGenerator() { return new SunGenerator; };
	Optimizer* CreateOptimizer() { return new SunOptimizer; };
};
void ScanParse()
{
	if (g_iType == SUN)
	{
		SunScanner scan;
		SunParser parser;
	}
	else
	{
		HPScanner scan;
		HPParser parser;
	}
};
void CodeGenOptimize()
{
	if (g_iType == SUN)
	{
		SunGenerator gen;
		SunOptimizer opti;
	}
	else
	{
		HPGenerator gen;
		HPOptimizer opti;
	}
};

void main()
{
	g_iType = SUN;
	CompilerFactory *g_pFactory;
	if (g_iType == HP)
	{
		g_pFactory = new HPCompilerFactory();
	}
	else
	{
		g_pFactory = new SunCompilerFactory();
	}
	Scanner* pScanner = g_pFactory->CreateScanner();
	Parser* pParser = g_pFactory->CreateParser();
	Generator* pGenerator = g_pFactory->CreateGenerator();
	Optimizer* pOpti = g_pFactory->CreateOptimizer();
}
