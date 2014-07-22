##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=ece454a4server
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4"
ProjectPath            := "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Harun
Date                   :=14-07-22
CodeLitePath           :="/home/mhderici/.codelite"
LinkerName             :=gcc
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=gcc
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             := -g $(Preprocessors)
LinkOptions            :=  
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=
LibPath                := "$(LibraryPathSwitch)." 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/custom_sample_server_app$(ObjectSuffix) $(IntermediateDirectory)/helper$(ObjectSuffix) $(IntermediateDirectory)/mybind$(ObjectSuffix) $(IntermediateDirectory)/server_stub$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.c $(IntermediateDirectory)/main$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/main.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/main.c"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/main.c"

$(IntermediateDirectory)/custom_sample_server_app$(ObjectSuffix): custom_sample_server_app.c $(IntermediateDirectory)/custom_sample_server_app$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/custom_sample_server_app.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/custom_sample_server_app$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/custom_sample_server_app$(DependSuffix): custom_sample_server_app.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/custom_sample_server_app$(ObjectSuffix) -MF$(IntermediateDirectory)/custom_sample_server_app$(DependSuffix) -MM "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/custom_sample_server_app.c"

$(IntermediateDirectory)/custom_sample_server_app$(PreprocessSuffix): custom_sample_server_app.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/custom_sample_server_app$(PreprocessSuffix) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/custom_sample_server_app.c"

$(IntermediateDirectory)/helper$(ObjectSuffix): helper.c $(IntermediateDirectory)/helper$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/helper.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/helper$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/helper$(DependSuffix): helper.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/helper$(ObjectSuffix) -MF$(IntermediateDirectory)/helper$(DependSuffix) -MM "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/helper.c"

$(IntermediateDirectory)/helper$(PreprocessSuffix): helper.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/helper$(PreprocessSuffix) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/helper.c"

$(IntermediateDirectory)/mybind$(ObjectSuffix): mybind.c $(IntermediateDirectory)/mybind$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/mybind.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/mybind$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mybind$(DependSuffix): mybind.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/mybind$(ObjectSuffix) -MF$(IntermediateDirectory)/mybind$(DependSuffix) -MM "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/mybind.c"

$(IntermediateDirectory)/mybind$(PreprocessSuffix): mybind.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mybind$(PreprocessSuffix) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/mybind.c"

$(IntermediateDirectory)/server_stub$(ObjectSuffix): server_stub.c $(IntermediateDirectory)/server_stub$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/server_stub.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/server_stub$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/server_stub$(DependSuffix): server_stub.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/server_stub$(ObjectSuffix) -MF$(IntermediateDirectory)/server_stub$(DependSuffix) -MM "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/server_stub.c"

$(IntermediateDirectory)/server_stub$(PreprocessSuffix): server_stub.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/server_stub$(PreprocessSuffix) "/home/mhderici/ece454/labs/lab2hub/DistributedCode/A4/ece454a4/server_stub.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/custom_sample_server_app$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/custom_sample_server_app$(DependSuffix)
	$(RM) $(IntermediateDirectory)/custom_sample_server_app$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/helper$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/helper$(DependSuffix)
	$(RM) $(IntermediateDirectory)/helper$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mybind$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mybind$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mybind$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/server_stub$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/server_stub$(DependSuffix)
	$(RM) $(IntermediateDirectory)/server_stub$(PreprocessSuffix)
	$(RM) $(OutputFile)


