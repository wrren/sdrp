solution "SDRP"
	configurations { "Debug", "Release" }
	
	project "SDRP"
		kind 		"StaticLib"
		language 	"C++"
		
		files 		{ 
			"../include/**.h",
			"../include/**.cpp"
		}
		
		includedirs	{ "../include/" }
				
		if _ACTION == "gmake" then
			location "unix"
		elseif _ACTION == "vs2010" then
			location "windows"
		elseif _ACTION == "vs2008" then
			location "windows"
		elseif _ACTION == "xcode4" then
			location "osx"
		end
