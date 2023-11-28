# lua静态库
add_library(lualib STATIC 
    lua/lzio.c
    lua/lctype.c
    lua/lopcodes.c
    lua/lmem.c
    lua/lundump.c
    lua/ldump.c
    lua/lstate.c
    lua/lgc.c
    lua/llex.c
    lua/lcode.c
    lua/lparser.c
    lua/ldebug.c
    lua/lfunc.c
    lua/lobject.c
    lua/ltm.c
    lua/lstring.c
    lua/ltable.c
    lua/ldo.c
    lua/lvm.c
    lua/lapi.c
    lua/lauxlib.c
    lua/lbaselib.c
    lua/lcorolib.c
    lua/ldblib.c
    lua/liolib.c
    lua/lmathlib.c
    lua/loadlib.c
    lua/loslib.c
    lua/lstrlib.c
    lua/ltablib.c
    lua/lutf8lib.c
    lua/linit.c)
set_property(TARGET lualib PROPERTY FOLDER "third_party")

# lua交互式解释器
add_executable(lua lua/lua.c)
target_link_libraries(lua lualib)
set_property(TARGET lua PROPERTY FOLDER "third_party")

# WTL示例程序
option(WITH_WTL "WTL samples" OFF)
if(${WITH_WTL} STREQUAL "ON")
    SET(THIRD_PARTY_ROOT_PATH ${CMAKE_HOME_DIRECTORY}/third_party/)
    INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/WTL/Include)

    add_definitions(-DUNICODE -D_UNICODE)

    add_executable(Aero WIN32 
        WTL/Samples/Aero/AboutDlg.h
        WTL/Samples/Aero/Aero.cpp
        WTL/Samples/Aero/Aero.h
        WTL/Samples/Aero/Aero.rc
        WTL/Samples/Aero/AeroView.h
        WTL/Samples/Aero/MainFrm.h
        WTL/Samples/Aero/resource.h
        WTL/Samples/Aero/stdafx.cpp
        WTL/Samples/Aero/stdafx.h
        )

    add_executable(Alpha WIN32 
        WTL/Samples/Alpha/aboutdlg.h
        WTL/Samples/Alpha/Alpha.cpp
        WTL/Samples/Alpha/Alpha.h
        WTL/Samples/Alpha/Alpha.rc
        WTL/Samples/Alpha/mainfrm.h
        WTL/Samples/Alpha/readme.txt
        WTL/Samples/Alpha/resource.h
        WTL/Samples/Alpha/stdafx.cpp
        WTL/Samples/Alpha/stdafx.h
        WTL/Samples/Alpha/view.h
        )

    add_executable(BmpView WIN32 
        WTL/Samples/BmpView/BmpView.cpp
        WTL/Samples/BmpView/BmpView.rc
        WTL/Samples/BmpView/list.h
        WTL/Samples/BmpView/mainfrm.h
        WTL/Samples/BmpView/props.h
        WTL/Samples/BmpView/resource.h
        WTL/Samples/BmpView/stdafx.cpp
        WTL/Samples/BmpView/stdafx.h
        WTL/Samples/BmpView/view.h
        )

    add_executable(GuidGen WIN32 
        WTL/Samples/GuidGen/aboutdlg.h
        WTL/Samples/GuidGen/GuidGen.cpp
        WTL/Samples/GuidGen/GuidGen.rc
        WTL/Samples/GuidGen/maindlg.h
        WTL/Samples/GuidGen/resource.h
        WTL/Samples/GuidGen/stdatl.cpp
        WTL/Samples/GuidGen/stdatl.h
        )

    add_executable(MDIDocVw WIN32 
        WTL/Samples/MDIDocVw/mainfrm.h
        WTL/Samples/MDIDocVw/MDI.cpp
        WTL/Samples/MDIDocVw/MDI.rc
        WTL/Samples/MDIDocVw/resource.h
        WTL/Samples/MDIDocVw/stdafx.cpp
        WTL/Samples/MDIDocVw/stdafx.h
        )

    add_executable(MemDlg WIN32 
        WTL/Samples/MemDlg/AboutDlg.h
        WTL/Samples/MemDlg/AboutDlgIndirect.h
        WTL/Samples/MemDlg/MainFrm.h
        WTL/Samples/MemDlg/MemDlg.cpp
        WTL/Samples/MemDlg/MemDlg.rc
        WTL/Samples/MemDlg/resource.h
        WTL/Samples/MemDlg/stdafx.cpp
        WTL/Samples/MemDlg/stdafx.h
        WTL/Samples/MemDlg/View.h
        )

    add_executable(MTPad WIN32 
        WTL/Samples/MTPad/aboutdlg.h
        WTL/Samples/MTPad/finddlg.h
        WTL/Samples/MTPad/mainfrm.h
        WTL/Samples/MTPad/MTPad.cpp
        WTL/Samples/MTPad/MTPad.h
        WTL/Samples/MTPad/MTPad.rc
        WTL/Samples/MTPad/resource.h
        WTL/Samples/MTPad/stdatl.cpp
        WTL/Samples/MTPad/stdatl.h
        WTL/Samples/MTPad/view.h
        )

    add_executable(TabBrowser WIN32 
        WTL/Samples/TabBrowser/AboutDlg.h
        WTL/Samples/TabBrowser/AddressCombo.h
        WTL/Samples/TabBrowser/BrowserView.h
        WTL/Samples/TabBrowser/CustomTabView.h
        WTL/Samples/TabBrowser/MainFrm.h
        WTL/Samples/TabBrowser/OpenDlg.h
        WTL/Samples/TabBrowser/resource.h
        WTL/Samples/TabBrowser/stdafx.cpp
        WTL/Samples/TabBrowser/stdafx.h
        WTL/Samples/TabBrowser/TabBrowser.cpp
        WTL/Samples/TabBrowser/TabBrowser.h
        WTL/Samples/TabBrowser/TabBrowser.rc
        WTL/Samples/TabBrowser/WindowsDlg.h
        )

    add_executable(WTLExplorer WIN32 
        WTL/Samples/WTLExplorer/ExplorerCombo.h
        WTL/Samples/WTLExplorer/MainFrm.cpp
        WTL/Samples/WTLExplorer/mainfrm.h
        WTL/Samples/WTLExplorer/resource.h
        WTL/Samples/WTLExplorer/ShellMgr.cpp
        WTL/Samples/WTLExplorer/ShellMgr.h
        WTL/Samples/WTLExplorer/stdafx.cpp
        WTL/Samples/WTLExplorer/stdafx.h
        WTL/Samples/WTLExplorer/WTLExplorer.cpp
        WTL/Samples/WTLExplorer/WTLExplorer.rc
        )
        
    # TODO:MTPad7
    # TODO:Wizard97Test

    set_property(TARGET Aero PROPERTY FOLDER "third_party/wtl_samples")
    set_property(TARGET Alpha PROPERTY FOLDER "third_party/wtl_samples")
    set_property(TARGET BmpView PROPERTY FOLDER "third_party/wtl_samples")
    set_property(TARGET GuidGen PROPERTY FOLDER "third_party/wtl_samples")
    set_property(TARGET MDIDocVw PROPERTY FOLDER "third_party/wtl_samples")
    set_property(TARGET MemDlg PROPERTY FOLDER "third_party/wtl_samples")
    set_property(TARGET MTPad PROPERTY FOLDER "third_party/wtl_samples")
    set_property(TARGET TabBrowser PROPERTY FOLDER "third_party/wtl_samples")
    set_property(TARGET WTLExplorer PROPERTY FOLDER "third_party/wtl_samples")
endif()

