library(xml2)
library(stringr)



LINUX = TRUE


if(LINUX)
{
  platform = "(Linux)"
  path = "/home/remi/Project/WBSFModels/build/"
}else
{
  platform = "(Windows)"
  path = "E:/ProjectCP/WBSFModels/build/"
}

workspace_str = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>
<CodeBlocks_workspace_file>
	<Workspace title=\"Workspace\">
	</Workspace>
</CodeBlocks_workspace_file>"

workspace = read_xml(workspace_str)
workspace_parent = xml_find_first(workspace, "//Workspace")



#Find all vcxproj files
vcxprojets = list.files(path = paste0(path,"msvc/"), pattern = ".vcxproj$", recursive = FALSE, full.names = TRUE)


for( i in 1:length(vcxprojets))
{
#i=1

  ###################################################################################
  #Initialisation
  
  #Model title
	file_title = sub("\\.[^.]+$", "", basename(vcxprojets[i]))
	file_name_out = sprintf( "%s%s.cbp", file_title,platform);
	file_path_out = sprintf( "%scb/%s", path,file_name_out);
	print(file_title)

	
	#read template code::block project file (xml file)
	template <- read_xml(paste0(path,"cb/Template",platform,".cbp"))
	
	#read current vcx project (xml file)
	vcxproj <- read_xml(vcxprojets[i])
	#remove namespace
	vcxproj = xml_ns_strip(vcxproj)
	
	#change project name
	project_name = xml_text(xml_find_first(vcxproj, "//ProjectName"))
	if(is.na(project_name))
		project_name=file_title
	
	
	
	###################################################################################	
	#replace title and targets
	xml_title = xml_find_first(template, "//Project/Option[@title]")
	xml_attr(xml_title, "title") <- project_name
	
	
	#replace targets (ie Debug/Release)
	xml_outputs = xml_find_all(template, "//Build/Target/Option[@output]")
	for( j in 1:length(xml_outputs) )
	{
		
		output = xml_attr(xml_outputs[j], "output")
		output = sub("DegreeDay", project_name, output)
		xml_attr(xml_outputs[j], "output") <- output
	}
	
	
	###################################################################################	
	#replace compiled files and include directories
	
	#find all compiled files
	vcxproj_include = xml_attr(xml_find_all(vcxproj, ".//ItemGroup/ClInclude"), "Include")
	vcxproj_include = c( vcxproj_include,  xml_attr(xml_find_all(vcxproj, ".//ItemGroup/ClCompile"), "Include") );

	parent_node <- xml_find_first(template, "//Project")
	# Add all compiled files to the parent
	for( j in 1:length(vcxproj_include) )
	{
		xml_add_child(parent_node, "Unit", filename=vcxproj_include[j])
	}


	#change Additional Directories
	IncludeDirectories = xml_text(xml_find_first(vcxproj, ".//AdditionalIncludeDirectories"))
	
	#remove local include directory
	IncludeDirectories = sub(";C:\\\\local\\\\boost_1_89_0", "", IncludeDirectories)
	IncludeDirectories = sub(";C:\\\\local\\\\boost_1_89_0", "", IncludeDirectories)
	IncludeDirectories = sub(";C:\\\\local\\\\gdal-3-11-3", "", IncludeDirectories)
	IncludeDirectories = sub(";C:\\\\local\\\\gsl-2-4-0", "", IncludeDirectories)
	IncludeDirectories = unlist(str_split(IncludeDirectories, ";"))

  #Add include directory
	parent_node <- xml_find_all(template, "//Build/Target/Compiler")
	
	for( j in 1:length(parent_node) )
	{
		for(k in 1:length(IncludeDirectories) )
			xml_add_child(parent_node, "Add", directory=IncludeDirectories[k])
	}

	###################################################################################	
	#Add dependencies
	
	#special case of EntomophagaMaimaiga (need gsl)
	if(file_title=="EntomophagaMaimaiga")
	{
		parent_node <- xml_find_all(template, "//Build/Target/Linker")
		for( j in 1:length(parent_node) )
		{
			xml_add_child(parent_node, "Add", library="libgsl.a")
		}
	}
	
	#Special case of WaterBalance (need gdal)
	if(file_title=="WaterBalance")
	{
		parent_node <- xml_find_all(template, "//Build/Target/Linker")
		for( j in 1:length(parent_node) )
		{
			xml_add_child(parent_node, "Add", library="libgdal.a")
		}
	}

	###################################################################################	
	#Save Code::Block project
	write_xml(template, file_path_out)
	
	
	#add this project to the workspace
	xml_add_child(workspace_parent, "Project", filename=file_name_out)
	
	# View the modified document
	#print(parent_node)
	
}




#save worspace 
write_xml(workspace, sprintf( "%scb/AllModels%s.workspace", path,platform))


