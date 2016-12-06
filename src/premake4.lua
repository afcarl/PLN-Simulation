solution "PolynomialWars"
    --Uncomment next line to build a debug version.
    -- configurations { "Debug" } 
    configurations { "Release", "macosx", "windows", "linux" }
    --For debug, remember to remove -O2 build option.
    --Also uncomment Symbols flag. 

    location("build_"..os.get())
    targetdir("build_"..os.get())

    project "pln"
        kind "ConsoleApp"
        language "C++"
        files
        { 
            "src/**.h", 
            "src/**.hpp",
            "src/**.inl",
            "src/**.cpp"
        }

        includedirs { "lib/SFML/"..os.get().."/include/",
                      "src/", 
                      "src/**" }

        libdirs { "lib/SFML/"..os.get().."/lib" }
        linkoptions { "-m32" } 
        buildoptions { "-m32", "-O0", "-std=c++11", "-Wall", "-pedantic"} 

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

