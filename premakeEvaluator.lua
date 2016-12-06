solution "EvaluatorAPI"
    --configurations { "Debug" } 
    configurations { "Release", "macosx", "windows", "linux" }

    location("build_"..os.get())
    targetdir("build_"..os.get())

    project "evaluator"
        kind "SharedLib"
        language "C++"
        files
        {
            --"D:/7_Projects/pln/src/LispAPI.h",
            --"D:/7_Projects/pln/src/LispAPI.cpp"
            "src/**.h", 
            "src/**.hpp",
            "src/**.inl",
            "src/**.c",
            "src/**.cpp"
        }
        excludes { "src/NEAT/**",
                   "src/FitnessEvaluator.hpp",
                   "src/FitnessEvaluator.cpp",
                 }


        -- includedirs { "src/" } 

        includedirs { "lib/SFML/"..os.get().."/include/",
                      "src/", 
                      "src/**" }

        libdirs { "lib/SFML/"..os.get().."/lib" }
        linkoptions { "-m32" } 
        buildoptions { "-m32", "-O2", "-std=c++11", "-Wall", "-pedantic"} 

        buildoptions { "-ffast-math" }

        -- buildoptions { "-g", "-gdwarf-2"}
                                            
        configuration "windows"
            defines "SFML_STATIC" 
            --flags { "Symbols" } --for Debug
            links
            {
                "sfml-network-s-d",
                "sfml-graphics-s-d",
                "sfml-window-s-d",
                "sfml-system-s-d",
                "opengl32",
                "winmm",
                "gdi32",
                "freetype",
                "jpeg",
            }
            excludes { "src/**/linux/**", "src/**/macosx/**" }

