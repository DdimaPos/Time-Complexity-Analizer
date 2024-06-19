#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

struct Function {
    std::string returnType;
    std::string name;
    std::string parameters;
    std::vector<std::string> code;
};

class FunctionTreeNode {
public:
    Function function;
    std::vector<FunctionTreeNode*> children;

    FunctionTreeNode(const Function& func) : function(func) {}
    
    // Add a child node to this function
    void addChild(FunctionTreeNode* child) {
        children.push_back(child);
    }

    // Print the function tree recursively
    void print(int indent = 0) {
        for (int i = 0; i < indent; i++) {
            std::cout << "  ";
        }
        std::cout << function.name << " (calls): ";
        for (FunctionTreeNode* child : children) {
            std::cout << child->function.name << " ";
        }
        std::cout << std::endl;
        for (FunctionTreeNode* child : children) {
            child->print(indent + 1);
        }
    }
};

class FunctionTree {
private:
    std::unordered_map<std::string, FunctionTreeNode*> nodes;

public:
    FunctionTree(const std::vector<Function>& functions) {
        // Create nodes for each function
        for (const Function& function : functions) {
            nodes[function.name] = new FunctionTreeNode(function);
        }
        
        // Link function calls
        for (const Function& function : functions) {
            FunctionTreeNode* node = nodes[function.name];
            for (const std::string& line : function.code) {
                // Check for function calls in the line
                for (const auto& [name, childNode] : nodes) {
                    if (line.find(name + "(") != std::string::npos) {
                        node->addChild(childNode);
                    }
                }
            }
        }
    }

    // Get the root node of the function tree
    FunctionTreeNode* getRoot() {
        // Assuming main function is the root
        return nodes["main"];
    }
};

int main() {
    // Define some functions with their code
    std::vector<Function> functions = {
        {
            "int", "main", "(int argc, char* argv[])",
            {
                "int a = add(3, 5);",
                "int b = subtract(10, 5);",
                "printResult(a);",
                "printResult(b);"
            }
        },
        {
            "int", "add", "(int a, int b)",
            {
                "return a + b;"
            }
        },
        {
            "int", "subtract", "(int a, int b)",
            {
                "return a - b;"
            }
        },
        {
            "void", "printResult", "(int result)",
            {
                "std::cout << result << std::endl;"
            }
        }
    };

    // Construct the function tree
    FunctionTree functionTree(functions);
    FunctionTreeNode* root = functionTree.getRoot();

    // Print the function tree
    std::cout << "Function Call Tree:" << std::endl;
    root->print();

    return 0;
}
