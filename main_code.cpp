#include <iostream> 
#include <string> 
#include <vector> 
#include <fstream>
#include <sstream>
#include <regex>
#include <thread>
#include <chrono>
using std::cout; 
using std::cin;
using std::endl;
#define RESET   "\033[0m"
#define BOLDGREEN   "\033[1m\033[32m"
#define RED     "\033[31m"

struct Function{//structure to store info about functions
    std::string returnType;
    std::string name;
    std::string parameters;
    std::vector<std::string> code;
};

struct complexityNode{//structure to hold  the tree nodes
    std::string complexity;//Lin-On; Log-logn;con-O1 
    std::string correspondingCode;
    std::vector<complexityNode> subnodes;
};

struct biggestComplexity{//structure to hold info about the maximum complexity calls
    std::string complexity;
    std::string correspondingCode;
};
void displayWithDelay(std::string message, int pause){
    for (int k = 0; k != message.size(); ++k) {
        std::cout << message[k]<< std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    }
    cout<<endl;
}
std::string checkAlphaNumeric(const std::string& lexicon) {
    const std::string alphaRegex = "^[a-zA-Z.]*$";
    const std::string numericRegex = "^-?[0-9.]*$";
    const std::string alphaNumericRegex = "^[a-zA-Z0-9.]*$";

    std::string trimmedLexicon = lexicon;
    trimmedLexicon.erase(std::remove_if(trimmedLexicon.begin(), trimmedLexicon.end(), [](unsigned char x){return std::isspace(x);}), trimmedLexicon.end());

    std::regex numericPattern(numericRegex);
    std::regex alphaNumericPattern(alphaNumericRegex);

    if (std::regex_match(trimmedLexicon, numericPattern)) {
        return "num";
    } else if (std::regex_match(trimmedLexicon, alphaNumericPattern)) {
        if (std::regex_match(trimmedLexicon.substr(0, 1), std::regex(alphaRegex))) {
            return "var";
        } else {
            return "err";
        }
    } else {
        return "err";
    }
}

std::string evaluateForLoopElements(std::string initValue, std::string rangeStop, std::string rangeOp, std::string inc){
    std::string initValueType=checkAlphaNumeric(initValue);
    if (initValue=="var"){
        std::string rangeStopType = checkAlphaNumeric(rangeStop);
        if(rangeStopType == "var") {
            return "con";
        }
        else if(rangeStopType == "num") {
            if(rangeOp == "<=" || rangeOp == "<") {
                return "err";
            }
            else if(rangeOp == ">=" || rangeOp == ">") {
                if(inc == "--" || inc == "-=") {
                    return "lin";
                }
                else if(inc == "/=") {
                    return "log";
                }
                else {
                    return "err";
                }
            }
        }
        else {
            return "err";
        }
    }else{
        if(initValueType == "num") {
            std::string rangeStopType = checkAlphaNumeric(rangeStop);
            if(rangeStopType == "var") {
                if(rangeOp == "<=" || rangeOp == "<") {
                    if(inc == "++" || inc == "+=") {
                        return "lin";
                    }
                    else if(inc == "*=") {
                        float pf = std::stof(initValue);
                        if(pf > 0) {
                            return "log";
                        }
                        else {
                            return "err";
                        }
                    }
                    else {
                        return "err";
                    }
                }
                else if(rangeOp == ">=" || rangeOp == ">") {
                    return "err";
                }
            }
            else if(rangeStopType == "num") {
                return "con";
            }
            else {
                return "err";
            }
        }
    }
    return "err";
}
// Function to evaluate operations
std::string evaluateOperation(const std::string& operation, const std::string& rangeStop, const std::string& rangeOp, const std::string& op, std::string initValue) {
    std::string operationDone;
    
    if (op == "=") {
        // Handle special case for "=" operation
        operationDone = "lazy";
    } else if (op == "++" || op == "--") {
        // Handle increment and decrement operations
        std::string lex = operation.substr(0, operation.find(op));
        std::string typeCheck = checkAlphaNumeric(lex);
        if (typeCheck== "var") {
            // Evaluate for loop elements
            operationDone = evaluateForLoopElements(initValue, rangeStop, rangeOp, op);
        } else {
            operationDone = "err";
        }
    } else if (op == "+=" || op == "-=") {
        //handle addition and subtraction assignments
        std::vector<std::string> operationSplit;
        size_t pos = operation.find(op);
        std::string leftLex = operation.substr(0, pos);
        std::string rightLex = operation.substr(pos + op.length());
        //Check types and conditions
        std::string leftTypeCheck = checkAlphaNumeric(leftLex);
        std::string rightTypeCheck = checkAlphaNumeric(rightLex);
        
        if(leftTypeCheck=="var"){
            if(rightTypeCheck=="num"){
                rightLex.erase(0, rightLex.find_first_not_of(' '));
                //convert string to integer
                int rightLexVal = std::stoi(rightLex);
                //print the integer value
                //std::cout << "rightLexVal: " << rightLexVal << std::endl;
                if(rightLexVal > 0) {
                    operationDone = evaluateForLoopElements(initValue, rangeStop, rangeOp, op);
                }
                else {
                    operationDone = "err";
                }
            }else{
                operationDone="err";
            }
        }else{
            operationDone="err";
        }
    } else if (op == "*=" || op == "/=") {
        //handle multiplication and division assignments
        std::vector<std::string> operationSplit;
        size_t pos = operation.find(op);
        std::string leftLex = operation.substr(0, pos);
        std::string rightLex = operation.substr(pos + op.length());
        //check types and conditions
        std::string leftTypeCheck = checkAlphaNumeric(leftLex);
        std::string rightTypeCheck = checkAlphaNumeric(rightLex);
        //check types and conditions
        if(leftTypeCheck=="var"){
            if(rightTypeCheck=="num"){
                rightLex.erase(0, rightLex.find_first_not_of(' '));
                //convert string to integer
                int rightLexVal = std::stoi(rightLex);
                //std::cout << "rightLexVal: " << rightLexVal << std::endl;
                if(rightLexVal > 1) {
                    operationDone = evaluateForLoopElements(initValue, rangeStop, rangeOp, op);
                }
                else {
                    operationDone = "err";
                }
            }else{
                operationDone="err";
            }
        }else{
            operationDone="err";
        }
    }
    return operationDone;
}

std::string semiColonSplit(std::string trimmedFor){
    std::vector<std::string> comparisonOperators = {">=", "<=", "==", "!=", ">", "<"};
    std::vector<std::string> operations = {"+=", "-=", "/=", "*=", "++", "--", "="};
    std::vector<std::string> arithOperations = {"+", "-", "/", "*"};
    
    std::string complexity;
    std::vector<std::string> forLogic;
    std::istringstream iss(trimmedFor);
    std::string token;
    while (std::getline(iss, token, ';')) {
        forLogic.push_back(token);
    }

    if(forLogic.size()!=3){
        return "ERR";
    }

    std::string varDeclaration=forLogic[0];
    std::string range=forLogic[1];
    std::string operation=forLogic[2];

    std::string initValue=varDeclaration.erase(0, varDeclaration.find('=')+1);
    initValue.erase(0, initValue.find_first_not_of(' '));
    

    std::string rangeStop;
    std::string rangeOp;///need to assign range stop and range op

    for(const auto& opr : comparisonOperators){
        if(range.find(opr)!=std::string::npos){
            rangeOp=opr;
            rangeStop=range.erase(0, range.find(opr)+1);
            break;
        }
    }
    /////////////////////////start cooking
    std::string operationDone;
    for (const auto& op : operations) {
        if (operation.find(op) != std::string::npos) {
            operationDone = evaluateOperation(operation, rangeStop, rangeOp, op, initValue);
            break;
        }
        operationDone = "err";
    }
    ///////////////////////
    return operationDone;
}
std::string parseFor(std::string line){
    std::string trimmedFor = line;
    trimmedFor.erase(0, trimmedFor.find_first_not_of(" \t"));//trims from line[0] to position where it can't find " \t"
    trimmedFor.erase(trimmedFor.find_last_not_of(" \t")+1);
    trimmedFor.erase(0, trimmedFor.find("(")+1);
    trimmedFor.erase(trimmedFor.find(")"), trimmedFor.length());//check later mb 2 operations can be reduced
    
    return semiColonSplit(trimmedFor);  
}
////////functions to build the graph///////
std::string getComplexity(std::vector<Function> extractedFunctions, std::vector<std::string> innerCode,std::string funcName, complexityNode *root){
    std::string complexity="con";
    Function activeFunc;
    complexityNode activeNode;
    std::vector<std::string> functNames;
    int level=0;
    
    for (int i = 0; i<innerCode.size(); i++){
        level=0;
        if ((innerCode[i].find("for ") != std::string::npos) || (innerCode[i].find("for(") != std::string::npos)){//try to find for loops
            //handle commented section
            if(innerCode[i].find("//")<innerCode[i].find("for(")) continue;

            activeNode.complexity=parseFor(innerCode[i]);
            activeNode.correspondingCode=innerCode[i];
            std::vector<std::string> innerForCode;
            level++;i++;
            
            //get inner code of for 
            while (level>0){
                innerForCode.push_back(innerCode[i]);
                if (innerCode[i].find("{") != std::string::npos){
                    level++;
                }
                if (innerCode[i].find("}") != std::string::npos)
                {
                    level--;
                }
                i++;
            }
            i--;
            innerForCode.pop_back();//remove the last line that should contain only '}'
            getComplexity(extractedFunctions, innerForCode, "for", &activeNode);//get complexity
            root->subnodes.push_back(activeNode);
            
        }
        for(int j = 0; j<extractedFunctions.size();j++){
            if((innerCode[i].find(extractedFunctions[j].name)!= std::string::npos) && i>0){//finds a function name. last condition to avoid infinite recursion(handle as error)
                if( innerCode[i].find("//") < innerCode[i].find(extractedFunctions[j].name) ) continue;
                if (extractedFunctions[j].name==funcName){
                    activeNode.complexity="recursive";
                    activeNode.correspondingCode=innerCode[i];
                }else {
                    getComplexity(extractedFunctions, extractedFunctions[j].code, extractedFunctions[j].name, &activeNode);
                    activeNode.complexity="con";
                    activeNode.correspondingCode=innerCode[i];
                }
                root->subnodes.push_back(activeNode);

            }
        }
        activeNode.subnodes.clear();
    }
    return complexity;

}
///////////

//////////function to get the maximum complexityt////////

//recursive function to find the path with maximum growth rate
void findMaxGrowthPath(const complexityNode& node, std::vector<biggestComplexity>& maxPath,
                       std::vector<biggestComplexity>& currentPath, int& maxLinearCount,
                       int& maxLogCount, std::vector<biggestComplexity>& bestPath) {
    //update current path
    currentPath.push_back({node.complexity, node.correspondingCode});

    //count linear and logarithmic nodes in the current path
    int linearCount = 0;
    int logCount = 0;
    for (const auto& n : currentPath) {
        if (n.complexity == "lin") linearCount++;
        else if (n.complexity == "log") logCount++;
    }

    //if the current path has more linear nodes, or the same number of linear nodes but more log nodes, update the best path
    if (linearCount > maxLinearCount || (linearCount == maxLinearCount && logCount > maxLogCount)) {
        maxLinearCount = linearCount;
        maxLogCount = logCount;
        bestPath = currentPath;
    }

    //recursively traverse child nodes
    for (const auto& child : node.subnodes) {
        findMaxGrowthPath(child, maxPath, currentPath, maxLinearCount, maxLogCount, bestPath);
    }

    currentPath.pop_back();
}

//////Function to display the result
void displayResult(std::vector<biggestComplexity> maxPath){
    int countLin=0;
    int countLog=0;
    int countErr=0;
    std::string wrongLine;
    for (const auto& node: maxPath){
        if (node.complexity=="lin")countLin++;
        if (node.complexity=="log")countLog++;
        if (node.complexity=="err"){
            countErr++;
            wrongLine=node.correspondingCode;
        }
    }
    if(countErr>0){
        cout<<RED;displayWithDelay("Error: wrong syntax or infinite loop", 10);cout<<RESET;
        cout<<RED;displayWithDelay(wrongLine, 20);cout<<RESET;
        system("pause");
        exit(1);
    }
    std::string complexity="O(";
    if(countLin>1)complexity.append("N^"+std::to_string(countLin));
    else if(countLin==1)complexity.append("N");
    if(countLog>1)complexity.append("log^"+std::to_string(countLog)+"(n)");
    else if(countLog==1)complexity.append("log(n)");
    if(countLin==0 && countLog==0)complexity.append("1");
    complexity.append(")");
    
    //cout<<complexity<<endl;
    cout<<BOLDGREEN;
    displayWithDelay(complexity, 80);cout<<RESET;
    displayWithDelay("Corresponding loops and function calls:", 20);
    
    for (const auto& node: maxPath){
        displayWithDelay(node.correspondingCode, 15);
        //cout<<node.correspondingCode<<endl;
    }

}

/////////function to extract the functions from source code//////////
std::vector<Function> extractFunctions(const std::vector<std::string> code){
    std::vector<Function> functions;//stores the functions and info about them
    std::string currentCode;
    bool inFunction = false;//state to keep track if current line is in function or not
    std::vector<std::string> functionCodeVector;
    std::string returnType, name, parameters;
    int braceDepth = 0;

    for(const auto& line: code){
        //trims the whitespaces before and after lines
        std::string trimmedLine = line;
        trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));//trims from line[0] to position where it can't find " \t"
        trimmedLine.erase(trimmedLine.find_last_not_of(" \t")+1);
        
        //check if line starts with a function definition
        if(!inFunction && trimmedLine.find('{') != std::string::npos){
            //possible start
            braceDepth++;
            size_t openBracePos = trimmedLine.find('{');
            size_t openParenthPos = trimmedLine.find('(');
            std::string functionHeader=trimmedLine.substr(0, openBracePos);
            std::istringstream headerStream(functionHeader);//declares a stream of functionHeader assigned to headerStream
            
            headerStream>>returnType>>name;
            size_t paramStart = name.find('(');
            //erase the parentheses after function name
            if(paramStart != std::string::npos){//additional verification
                parameters = trimmedLine.substr(openParenthPos, functionHeader.find(')'));
                name = name.substr(0, paramStart);
            }
            inFunction=true; //we are in function
            functionCodeVector.push_back(line);
            //functionStream<<line<<"\n"; //remove if need ony inner code of function

        }else if(inFunction){
            
            if(trimmedLine.find('{')!=std::string::npos){
                braceDepth++;
            }
                
            
            functionCodeVector.push_back(line);
            //functionStream<<line<<"\n";

            if (trimmedLine.find('}') != std::string::npos){
               
                braceDepth--;
                if(braceDepth==0){
                    Function function={returnType, name, parameters, functionCodeVector};
                    functions.push_back(function);
                    inFunction=false;
                    functionCodeVector.clear();
                }
                
            }
            
        }
    }
    return functions;
}
//////////functions to read the code//////////////

std::vector<std::string> readInputCode(){
    std::string str; 
    std::vector<std::string> s; 
    displayWithDelay("Enter the code that you want to be analysed(code is read until first empty line):", 5);
    //cout << "Enter the code that you want to be analysed(code is read until first empty line): " << endl; 
  
    while (getline(cin, str)) { 
        if (str.empty()) { 
            break; 
        } 
        s.push_back(str); 
    } 

    return s;
}

std::vector<std::string> readCodeFromFile(){
    std::vector<std::string> code;
    std::string line;
    std::string fileName;
    std::ifstream sourceFile;

    displayWithDelay("Write the name of file that you want to read:", 10);
    //cout<<"Write the name of file that you want to read: ";
    cin>>fileName;
    cin.ignore(10, '\n');
    sourceFile.open(fileName);
    if (!sourceFile.is_open()){
        cout<<RED<<"Something went wrong. Check the file name\n"<<RESET;system("pause");
        exit(0);
    }
    else{
        cout<<"File openned successfully\n";
    }
    while(getline(sourceFile, line)){
        code.push_back(line);
    }
    sourceFile.close();
    return code;
}
////////////////////////////////////////////// 
std::vector<std::string> codeReadingOption(){
    std::vector<std::string> code;
    int op=0;
    displayWithDelay("Choose the variant of reading the code\n 1. By introducing the code\n 2. Read from file", 5);
    cin>>op;
    cin.ignore(10, '\n');
    switch (op){
    case 1:
        code=readInputCode();
        break;
    case 2:
        code=readCodeFromFile();
        break;
    default:
        displayWithDelay("invalid operation\n", 10);
        //cout<<"invalid operation\n";
        code=codeReadingOption();
        break;
    }
    return code;
}
 
int main(){
     
    cout<<"████████╗██╗███╗░░░███╗███████╗  ░█████╗░░█████╗░███╗░░░███╗██████╗░██╗░░░░░███████╗██╗░░██╗██╗████████╗██╗░░░██╗\n";
    cout<<"╚══██╔══╝██║████╗░████║██╔════╝  ██╔══██╗██╔══██╗████╗░████║██╔══██╗██║░░░░░██╔════╝╚██╗██╔╝██║╚══██╔══╝╚██╗░██╔╝\n";
    cout<<"░░░██║░░░██║██╔████╔██║█████╗░░  ██║░░╚═╝██║░░██║██╔████╔██║██████╔╝██║░░░░░█████╗░░░╚███╔╝░██║░░░██║░░░░╚████╔╝░\n";
    cout<<"░░░██║░░░██║██║╚██╔╝██║██╔══╝░░  ██║░░██╗██║░░██║██║╚██╔╝██║██╔═══╝░██║░░░░░██╔══╝░░░██╔██╗░██║░░░██║░░░░░╚██╔╝░░\n";
    cout<<"░░░██║░░░██║██║░╚═╝░██║███████╗  ╚█████╔╝╚█████╔╝██║░╚═╝░██║██║░░░░░███████╗███████╗██╔╝╚██╗██║░░░██║░░░░░░██║░░░\n";
    cout<<"░░░╚═╝░░░╚═╝╚═╝░░░░░╚═╝╚══════╝  ░╚════╝░░╚════╝░╚═╝░░░░░╚═╝╚═╝░░░░░╚══════╝╚══════╝╚═╝░░╚═╝╚═╝░░░╚═╝░░░░░░╚═╝░░░\n";
    cout<<"\n";
    cout<<"░█████╗░░█████╗░██╗░░░░░░█████╗░██╗░░░██╗██╗░░░░░░█████╗░████████╗░█████╗░██████╗░\n";
    cout<<"██╔══██╗██╔══██╗██║░░░░░██╔══██╗██║░░░██║██║░░░░░██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗\n";
    cout<<"██║░░╚═╝███████║██║░░░░░██║░░╚═╝██║░░░██║██║░░░░░███████║░░░██║░░░██║░░██║██████╔╝\n";
    cout<<"██║░░██╗██╔══██║██║░░░░░██║░░██╗██║░░░██║██║░░░░░██╔══██║░░░██║░░░██║░░██║██╔══██╗\n";
    cout<<"╚█████╔╝██║░░██║███████╗╚█████╔╝╚██████╔╝███████╗██║░░██║░░░██║░░░╚█████╔╝██║░░██║\n";
    cout<<"░╚════╝░╚═╝░░╚═╝╚══════╝░╚════╝░░╚═════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░░╚════╝░╚═╝░░╚═╝\n";
    std::vector<std::string> code;
    std::string s; 
    code=codeReadingOption();//choose reading option and extract the code
    /*cout << "The code to work with: " << endl; 
    for (std::string& it : code) { 
        cout << it << endl; 
    } */
    std::vector<Function> extractedFunctions=extractFunctions(code);

    std::vector<std::string> innerCode;
    
    int mainFound=0;
    for(const auto& el: extractedFunctions){
        if(el.name=="main"){
            innerCode=el.code;
            mainFound=1;
            break;
        }
    }

    if (!mainFound){
        cout<<"No main function found";
        system("pause");
        exit(0);
    }
    
    
   
    complexityNode RootNode;
    RootNode.complexity="con";
    RootNode.correspondingCode="main()";
    getComplexity(extractedFunctions, innerCode, "main", &RootNode);

    std::vector<biggestComplexity> maxPath;
    std::vector<biggestComplexity> currentPath;
    int maxLinearCount = 0;
    int maxLogCount = 0;
    std::vector<biggestComplexity> bestPath;//stores the path with biggest complexity

    findMaxGrowthPath(RootNode, maxPath, currentPath, maxLinearCount, maxLogCount, bestPath);
    displayResult(bestPath);
    system("pause");
    return 0; 
}