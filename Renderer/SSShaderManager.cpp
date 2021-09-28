
#include "Core.h"
#include "SSShaderManager.h"
#include "SSName.h"
#include "SSShader.h"

#include <filesystem>
#include "simdjson.h"

void SSShaderManager::Initialize()
{
    // load shaderpermutation.json
    simdjson::dom::parser parser;
    simdjson::dom::element shaderdefines = parser.load("./Shader/ShaderPermutation.json");

	for(auto shader : shaderdefines["shaders"])
    {
        std::string_view shaderName = shader["name"].get_string().take_value();
        std::string_view filepath = shader["filepath"].get_string().take_value();

        SSCompileContext context;
        // macro defines
        if(!shader["defines"].error())
        {
            for(auto define : shader["defines"])
            {
                for(auto [k,v] : define.get_object())
                {
                    std::string_view definename = k.data();

                    std::string_view definition = v.get_string().take_value();

                    std::pair<std::string_view ,std::string_view> pair{definename, definition};

                    context.MacroDefines.push_back(pair);
                }
            }
        }

        // instanced vertex attribute
        if(!shader["instanced"].error())
        {
            for (auto instanced : shader["instanced"])
            {
                context.InstancedAttributes.push_back(instanced.get_string().take_value());
            }
        }

        if(filepath.find(".vs") != std::string_view::npos)
        {
            std::shared_ptr<SSVertexShader> vs = std::make_shared<SSVertexShader>();
            std::string cstrfilepath = filepath.data();
            std::wstring wfilepath;
            wfilepath.assign(cstrfilepath.begin(), cstrfilepath.end());

            if (vs->CompileFromFile(wfilepath, context) == true)
            {
                mVertexShaderMap[shaderName.data()] = vs;
            }
            else
            {
                check(false);
            }
        }
        else if(filepath.find(".ps") != std::string_view::npos)
        {
            std::shared_ptr<SSPixelShader> ps = std::make_shared<SSPixelShader>();
            std::string cstrfilepath = filepath.data();
            std::wstring wfilepath;
            wfilepath.assign(cstrfilepath.begin(), cstrfilepath.end());

            if (ps->CompileFromFile(wfilepath, context) == true)
            {
                mPixelShaderMap[shaderName.data()] = ps;
            }
            else
            {
                check(false);
            }
        }
    }

	for (auto& f : std::filesystem::directory_iterator("./Shader"))
	{
		std::string filename;

		std::wstring filepath = f.path().c_str();
		std::wstring wfilename = f.path().filename().c_str();

		filename.assign(wfilename.begin(), wfilename.end());		

		if (filename.find(".vs") != std::string::npos)
		{
			std::shared_ptr<SSVertexShader> vs = std::make_shared<SSVertexShader>();

			if (vs->CompileFromFile(filepath) == true)
			{
				mVertexShaderMap[filename] = vs;
			}
			else
			{
				check(false);
			}
		}
		else if (filename.find(".ps") != std::string::npos)
		{
			std::shared_ptr<SSPixelShader> ps = std::make_shared<SSPixelShader>();
			if (ps->CompileFromFile(filepath) == true)
			{
				mPixelShaderMap[filename] = ps;
			}
			else
			{
				check(false);
			}
		}
	}

}


void SSShaderManager::Shutdown()
{
	//
	for (auto& kvp : mVertexShaderMap)
	{
		//kvp.second->Destroy();
	}

	for (auto& kvp : mPixelShaderMap)
	{
		//kvp.second->Destroy();
	}
}


std::shared_ptr<SSVertexShader> SSShaderManager::GetVertexShader(SSName name)
{
	check(mVertexShaderMap.count(name) > 0);
	return mVertexShaderMap[name];
}

std::shared_ptr<SSPixelShader> SSShaderManager::GetPixelShader(SSName name)
{
	check(mPixelShaderMap.count(name) > 0);
	return mPixelShaderMap[name];
}