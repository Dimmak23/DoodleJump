# ? Let's keep features that we will freq. re-compile and add there something
# ? also explicitly telling that some features needs testing
set(INCLUDES "${CMAKE_SOURCE_DIR}/headers")

# * Libraries
set(LIB_APPLICATION "${INCLUDES}/application")

# * Core geometry features
set(SCREEN "${INCLUDES}/screen")
set(RECT "${INCLUDES}/rectangle")

# * Actors
set(ACT_FEATURES "${INCLUDES}/actor/interface")
set(ACT_PLAYER "${INCLUDES}/actor/player")

# * Modules
set(MOD_PARALLAX "${INCLUDES}/modules/parallax")
set(MOD_LOCATOR "${INCLUDES}/modules/locator")
set(MOD_PENGINE "${INCLUDES}/modules/physics_engine")
set(MOD_COLLIDER "${INCLUDES}/modules/collider")
set(MOD_SCENE "${INCLUDES}/modules/scene")
set(MOD_CAMERA "${INCLUDES}/modules/camera")
set(MOD_RSPAWN "${INCLUDES}/modules/spawn")
set(MOD_GUN "${INCLUDES}/modules/gun")

# * Abilities
set(ABL_JETPACK "${INCLUDES}/abilities/jet_pack")
set(ABL_WRMHL "${INCLUDES}/abilities/wormhole")
set(ABL_STEALTH "${INCLUDES}/abilities/stealth")

# * Levels
set(LVL "${INCLUDES}/level")
set(LVL_WORLD "${LVL}/world")
set(LVL_MAIN "${LVL}/level")
set(UNI_FEATURES "${LVL_WORLD}/interface")
set(LVL_SWORLD "${LVL_WORLD}/static")
set(LVL_DWORLD "${LVL_WORLD}/dynamic")

# * Wrappers
set(WR_FEATURES "${INCLUDES}/wrappers/interface")
set(WR_SPRITE "${INCLUDES}/wrappers/sprite")
set(WR_ANISPRITE "${INCLUDES}/wrappers/animated_sprite")

# * Widgets
set(SCR_BOARD "${INCLUDES}/score_board")

# * Projectiles
set(PT_AMMO "${INCLUDES}/projectiles/ammo")

set(UTILS "${INCLUDES}/utilities")
