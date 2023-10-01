import json

# TODO JSON SETTING FILE, 

def write_parameters(data, key):
    parameters = data[key]["parameters"]
    for parameterKey in parameters.keys():
        if type(parameters[parameterKey]) == bool:
            f.write(f'\tbool {parameterKey} = m_params["{parameterKey}"].toBool();\n')
        elif type(parameters[parameterKey]) == int:
            f.write(f'\tint16_t {parameterKey} = m_params["{parameterKey}"].toInt();\n')
        elif type(parameters[parameterKey]) == dict:
            f.write(f'\tQString {parameterKey} = m_params["{parameterKey}"].toObject()["type"].toString();\n')

    f.write("\t// TODO LOW LEVEL IMPLEMENTATION\n")


with open("actions.json") as jsonFile:
    data = json.load(jsonFile)
    keys = data.keys()

    f = open("generatedActions.cpp","w+")
    g = open("actionFactory.cpp")
    h = open("generatedActions.h","w+")
    f.write('#include "generatedActions.h"\n\n\n')

    h.write('#ifndef GENERATED_ACTIONS_H\n')
    h.write('#define GENERATED_ACTIONS_H\n\n')
    h.write('#include "strategyframework.h"\n\n')

    g.write("void ActionFactory::init(bool colorside)\n{\n")

    for key in keys:
        
        f.write(f"void {key}::onEntry(QEvent*)\n")
        f.write("{\n")
        write_parameters(data, key)
        f.write("}\n\n")
        f.write(f"void {key}::onExit(QEvent*)\n")
        f.write("{\n")
        write_parameters(data, key)
        f.write("}\n\n")
        f.write("// --------------------------------------------------------------------------------------\n\n")

        h.write(f'CREATE_ACTION_CLASS_EL({key}, "{key}");\n')
        h.write("\n")

        g.write('\tm_map.emplace("{key}", [](QState *parent, QJsonObject params, QString Tag){return new {key}(parent, params, Tag);});)\n')
    
    g.write("}\n\n")
    h.write('#endif')
    f.close
    h.close 