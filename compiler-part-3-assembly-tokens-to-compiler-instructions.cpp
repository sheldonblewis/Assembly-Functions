// takes assembly tokens and converts them to compiler instructions
// Part 3 of the compiler

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>

void formatError(const std::string &message) {
    std::cerr << "ERROR: " << message << std::endl;
    std::exit(1);
}

void compileLine(std::string instruction, const std::vector<int32_t> &params) {
    if (instruction == ".word") {
        instruction = "word";
    }

    std::cout << instruction;
    size_t paramCount = 0;
    for (int32_t param : params) {
        if (param < 0) {
            param += 65536;
        }
        std::cout << " " << param;
        ++paramCount;
    }

    while (paramCount < 3) {
        std::cout << " 0";
        ++paramCount;
    }
    std::cout << std::endl;
}

int main() {
    std::map<std::string, uint32_t> labelMap;
    std::vector<std::pair<std::string, uint32_t>> labelOrder;
    std::vector<std::pair<std::string, std::vector<std::string>>> instructions;
    uint32_t currentAddress = 0;

    std::string lastInstruction;
    std::vector<std::string> lastParams;
    bool expectingNewInstruction = true;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::stringstream lineParser(line);
        std::string firstWord;
        lineParser >> firstWord;

        if (firstWord == "LABEL") {
            std::string labelName;
            lineParser >> labelName;
            if (labelName.back() == ':') labelName.pop_back();
            if (labelMap.count(labelName)) {
                formatError("Duplicate label definition: " + labelName);
            }
            labelMap[labelName] = currentAddress;
            labelOrder.emplace_back(labelName, currentAddress);
            expectingNewInstruction = true;
        } else if (firstWord == "DIRECTIVE" || (firstWord == "ID" && expectingNewInstruction)) {
            std::string instruction;
            lineParser >> instruction;
            std::vector<std::string> params;
            std::string param;

            while (lineParser >> param) {
                params.push_back(param);
            }

            instructions.emplace_back(instruction, params);
            lastInstruction = instruction;
            lastParams = params;
            expectingNewInstruction = false;
            currentAddress += 4;
        } else if (firstWord == "ID" && !expectingNewInstruction) {
            std::string param;
            lineParser >> param;
            lastParams.push_back(param);
            instructions.back().second.push_back(param);
        } else if (firstWord == "REG") {
            std::string param;
            lineParser >> param;
            if (!param.empty() && param.front() == '$') {
                param = param.substr(1);
            }
            lastParams.push_back(param);
            instructions.back().second.push_back(param);
        } else if (firstWord == "DEC") {
            std::string param;
            lineParser >> param;
            lastParams.push_back(param);
            instructions.back().second.push_back(param);
        } else if (firstWord == "HEXINT") {
            std::string param;
            lineParser >> param;
            if (param.find("0x") == 0) {
                param = param.substr(2);
                lastParams.push_back(std::to_string(std::stoul(param, nullptr, 16)));
            }
            instructions.back().second.push_back(param);
        } else if (firstWord == "COMMA") {
            continue;
        } else if (firstWord == "NEWLINE") {
            expectingNewInstruction = true;
        }
    }

    for (const auto &[label, address] : labelOrder) {
        std::cerr << label << " " << address << std::endl;
    }

    uint32_t instructionAddress = 0;
    for (const auto &[instruction, params] : instructions) {
        if (instruction.empty()) continue;

        std::vector<int32_t> resolvedParams;

        for (const auto &param : params) {
            if (labelMap.count(param)) {
                if (instruction == "beq" || instruction == "bne") {
                    int32_t delta = static_cast<int32_t>(labelMap[param]) - static_cast<int32_t>(instructionAddress + 4);
                    resolvedParams.push_back(delta / 4);  // Convert to word offset
                } else {
                    resolvedParams.push_back(static_cast<int32_t>(labelMap[param]));
                }
            } else {
                std::string cleanedParam = param;
                if (cleanedParam.front() == '$') cleanedParam = cleanedParam.substr(1);
                if (cleanedParam.back() == ',') cleanedParam.pop_back();

                try {
                    resolvedParams.push_back(std::stoul(cleanedParam));
                } catch (const std::exception &e) {
                    formatError("Invalid parameter: " + cleanedParam);
                }
            }
        }

        compileLine(instruction, resolvedParams);
        instructionAddress += 4;
    }

    return 0;
}
