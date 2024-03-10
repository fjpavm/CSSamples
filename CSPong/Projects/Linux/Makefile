# GNU Makefile to create the executable and contents of
# CSPong for linux

PROJECT_NAME=CSPong
SOURCES = CSPong.cpp \
Common/TransitionSystem.cpp \
Game/Ball/BallControllerComponent.cpp \
Game/Camera/CameraTiltComponent.cpp \
Game/GameEntityFactory.cpp \
Game/GameState.cpp \
Game/GoalCeremonySystem.cpp \
Game/Paddle/AIControllerComponent.cpp \
Game/Paddle/TouchControllerComponent.cpp \
Game/Physics/DynamicBodyComponent.cpp \
Game/Physics/PhysicsSystem.cpp \
Game/Physics/StaticBodyComponent.cpp \
Game/Physics/TriggerComponent.cpp \
Game/ScoringSystem.cpp \
MainMenu/MainMenuState.cpp \
Splash/SplashState.cpp

OBJECTS = $(call source_to_object, $(SOURCES))
DEPENDENCIES = $(call object_to_depend, $(OBJECTS))

SOLUTION_DIR = .

MKDIR = mkdir -p

PLATFORM = Linux
SRC_DIR = ../../AppSource/
CHILLISOURCE_PROJECT_DIR = ../../ChilliSource/Projects/$(PLATFORM)
CHILLISOURCE_SRC_DIR = ../../ChilliSource/Source/
CORE_LIB_DIR = ../../ChilliSource/Libraries/Core/$(PLATFORM)
CRICKET_LIB_DIR = ../../ChilliSource/Libraries/CricketAudio/$(PLATFORM)
CHILLISOURCE_LIB_DIR = $(CHILLISOURCE_PROJECT_DIR)/csbin/$(PLATFORM)
OBJ_DIR = $(SOLUTION_DIR)/csobj/$(PLATFORM)/
OUT_DIR = $(SOLUTION_DIR)/csbin/$(PLATFORM)/
TARGET_ARCHITECTURE = x86_64
TARGET_VERSION = release

LIBS_LINUX = CSBase  sfml-window-s sfml-system-s crypto ssl curl GLEW
SHARED_LIBS_LINUX = z X11 Xrandr Xcursor GL udev asound ogg pthread

#CPPFLAGS += -fpermissive
# add compiler use c++11 flags
CPPFLAGS += -std=c++11 -std=gnu++11 
# add compiler debug flag for debug versions and debug defines
CPPFLAGS += $(if $(or $(call eq,$(TARGET_VERSION),debug),$(call eq,$(TARGET_VERSION),DEBUG)), -g -D_DEBUG -DDEBUG -DCS_ENABLE_DEBUG)
# add compiler flag for each architecture
CPPFLAGS += $(if $(call eq,$(TARGET_ARCHITECTURE),x86_64), -m64 )
CPPFLAGS += $(if $(call eq,$(TARGET_ARCHITECTURE),x86_32), -m32 )
# add needed include path
CPPFLAGS += -I $(SRC_DIR) -I $(CORE_LIB_DIR)/Headers -I $(CHILLISOURCE_SRC_DIR) 
CPPFLAGS += -include string -include cstring
# add platform define
CPPFLAGS += -DCS_TARGETPLATFORM_$(call to_upper,$(PLATFORM))
# add libChilliSource library flags 
CPPFLAGS += -Wl,-Bstatic
CPPFLAGS += -L$(CHILLISOURCE_LIB_DIR)/$(TARGET_ARCHITECTURE)/$(TARGET_VERSION) 
CPPFLAGS += -lChilliSource
# add dependency defines and library flags 
# NOTE:Always using release version of libraries even in debug build
CPPFLAGS += -DGLEW_STATIC -DSFML_STATIC
CPPFLAGS += -L$(CRICKET_LIB_DIR)/Libs/$(TARGET_ARCHITECTURE)/release 
CPPFLAGS += -lck
CPPFLAGS += -L$(CORE_LIB_DIR)/Libs/$(TARGET_ARCHITECTURE)/release 
CPPFLAGS += $(foreach lib,$(LIBS_$(call to_upper,$(PLATFORM))), -l$(lib))
# add shared libraries flag (assumed part of the system)
CPPFLAGS += -Wl,-Bdynamic
CPPFLAGS += $(foreach lib,$(SHARED_LIBS_$(call to_upper,$(PLATFORM))), -l$(lib))


# Helper Macros
eq = $(and $(findstring $(1),$(2)),$(findstring $(2),$(1)))
contains = $(findstring $(1),$(2))
to_upper = $(shell echo $(1) | tr '[a-z]' '[A-Z]')
# $(call source_to_object, source_file_list) 
source_to_object = $(subst .c,.o,$(filter %.c,$1)) \
                   $(subst .cpp,.o,$(filter %.cpp,$1))
# $(call source_to_depend, source_file_list) 
source_to_depend = $(subst .c,.d,$(filter %.c,$1)) \
                   $(subst .cpp,.d,$(filter %.cpp,$1))
# $(call object_to_depend, object_file_list) 
object_to_depend = $(subst .o,.d,$(filter %.o,$1)) 
# $(call make-depend,source-file,object-file,depend-file)
define make-depend
 $(COMPILE.cpp) -MM -MF $3 -MP -MT $2 $1
endef

OUT_PREFIX_DIR = $(OUT_DIR)$(TARGET_ARCHITECTURE)/$(TARGET_VERSION)
OBJ_PREFIX_DIR = $(OBJ_DIR)$(PROJECT_NAME)/$(TARGET_ARCHITECTURE)/$(TARGET_VERSION)
# $(call make_object_rule,source-file,object-file,depend-file)
define make_object_rule

$2 : $1
	$$(MKDIR) $$(dir $(2))
	$$(call make-depend, $1, $2, $3)
	$$(COMPILE.cpp) -c $1
	mv *.o $$(dir $(2)) || true
endef


#Target rules
all : $(OUT_PREFIX_DIR)/$(PROJECT_NAME) copy_resources

# include generated object dependencies
ifneq "$(MAKECMDGOALS)" "clean"
 $(foreach dependency,$(DEPENDENCIES),       \
    $(eval -include $(OBJ_PREFIX_DIR)/$(dependency))\
 ) 
endif


# rule to run ChiliSource project make 
.PHONY: $(CHILLISOURCE_PROJECT_DIR)
$(CHILLISOURCE_PROJECT_DIR):
	$(MAKE) --directory=$@ PLATFORM=$(PLATFORM) TARGET_ARCHITECTURE=$(TARGET_ARCHITECTURE) TARGET_VERSION=$(TARGET_VERSION) 

# cleanning need *.o for when errors interupt build midway
.PHONY: clean
clean :
	rm -r csobj || true
	rm *.o || true

.PHONY: clean_all
clean_all: clean
	$(MAKE) --directory=$(CHILLISOURCE_PROJECT_DIR) clean

# make object target rules
$(foreach source, $(SOURCES), \
 $(eval $(call make_object_rule, \
  $(SRC_DIR)/$(source), \
  $(addprefix $(OBJ_PREFIX_DIR)/,$(call source_to_object, $(source))), \
  $(addprefix $(OBJ_PREFIX_DIR)/,$(call source_to_depend, $(source))) \
  ))\
)

OBJ_PREFIX_DIR_OBJECTS = $(addprefix $(OBJ_PREFIX_DIR)/,$(OBJECTS))
$(OUT_PREFIX_DIR)/$(PROJECT_NAME) : $(OBJ_PREFIX_DIR_OBJECTS) $(CHILLISOURCE_PROJECT_DIR)
	$(MKDIR) $(OUT_PREFIX_DIR)
	$(LINK.cpp) -o $(OUT_PREFIX_DIR)/$(PROJECT_NAME) $(OBJ_PREFIX_DIR_OBJECTS) $(CPPFLAGS)

 

.PHONY : copy_resources
copy_resources :
#	python ../../Tools/Scripts/extract_headers.py -i "$(SRC_DIR)/" -o "$(OUT_DIR)Headers"
#	python ../../Tools/Scripts/extract_headers.py -i "$(PLATFORM_SRC_DIR)/" -o "$(OUT_DIR)PlatformHeaders"
#	cp -rf $(OUT_DIR)PlatformHeaders/* $(OUT_DIR)Headers/ || true
#	rm -rf $(OUT_DIR)PlatformHeaders
