QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = barblunder.ico

SOURCES += \
    Box2D/Collision/Shapes/b2ChainShape.cpp \
    Box2D/Collision/Shapes/b2CircleShape.cpp \
    Box2D/Collision/Shapes/b2EdgeShape.cpp \
    Box2D/Collision/Shapes/b2PolygonShape.cpp \
    Box2D/Collision/b2BroadPhase.cpp \
    Box2D/Collision/b2CollideCircle.cpp \
    Box2D/Collision/b2CollideEdge.cpp \
    Box2D/Collision/b2CollidePolygon.cpp \
    Box2D/Collision/b2Collision.cpp \
    Box2D/Collision/b2Distance.cpp \
    Box2D/Collision/b2DynamicTree.cpp \
    Box2D/Collision/b2TimeOfImpact.cpp \
    Box2D/Common/b2BlockAllocator.cpp \
    Box2D/Common/b2Draw.cpp \
    Box2D/Common/b2FreeList.cpp \
    Box2D/Common/b2Math.cpp \
    Box2D/Common/b2Settings.cpp \
    Box2D/Common/b2StackAllocator.cpp \
    Box2D/Common/b2Stat.cpp \
    Box2D/Common/b2Timer.cpp \
    Box2D/Common/b2TrackedBlock.cpp \
    Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    Box2D/Dynamics/Contacts/b2Contact.cpp \
    Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    Box2D/Dynamics/Joints/b2GearJoint.cpp \
    Box2D/Dynamics/Joints/b2Joint.cpp \
    Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    Box2D/Dynamics/b2Body.cpp \
    Box2D/Dynamics/b2ContactManager.cpp \
    Box2D/Dynamics/b2Fixture.cpp \
    Box2D/Dynamics/b2Island.cpp \
    Box2D/Dynamics/b2World.cpp \
    Box2D/Dynamics/b2WorldCallbacks.cpp \
    Box2D/Particle/b2Particle.cpp \
    Box2D/Particle/b2ParticleAssembly.cpp \
    Box2D/Particle/b2ParticleGroup.cpp \
    Box2D/Particle/b2ParticleSystem.cpp \
    Box2D/Particle/b2VoronoiDiagram.cpp \
    Box2D/Rope/b2Rope.cpp \
    applicationmodel.cpp \
    drinkview.cpp \
    gamelayer.cpp \
    glassware.cpp \
    liquidmodel.cpp \
    main.cpp \
    mainmenupage.cpp \
    mainwindow.cpp \
    barmodel.cpp \
    recipe.cpp \
    recipenote.cpp \
    settingsmenupage.cpp

HEADERS += \
    Box2D/Box2D.h \
    Box2D/Collision/Shapes/b2ChainShape.h \
    Box2D/Collision/Shapes/b2CircleShape.h \
    Box2D/Collision/Shapes/b2EdgeShape.h \
    Box2D/Collision/Shapes/b2PolygonShape.h \
    Box2D/Collision/Shapes/b2Shape.h \
    Box2D/Collision/b2BroadPhase.h \
    Box2D/Collision/b2Collision.h \
    Box2D/Collision/b2Distance.h \
    Box2D/Collision/b2DynamicTree.h \
    Box2D/Collision/b2TimeOfImpact.h \
    Box2D/Common/b2BlockAllocator.h \
    Box2D/Common/b2Draw.h \
    Box2D/Common/b2FreeList.h \
    Box2D/Common/b2GrowableBuffer.h \
    Box2D/Common/b2GrowableStack.h \
    Box2D/Common/b2IntrusiveList.h \
    Box2D/Common/b2Math.h \
    Box2D/Common/b2Settings.h \
    Box2D/Common/b2SlabAllocator.h \
    Box2D/Common/b2StackAllocator.h \
    Box2D/Common/b2Stat.h \
    Box2D/Common/b2Timer.h \
    Box2D/Common/b2TrackedBlock.h \
    Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    Box2D/Dynamics/Contacts/b2CircleContact.h \
    Box2D/Dynamics/Contacts/b2Contact.h \
    Box2D/Dynamics/Contacts/b2ContactSolver.h \
    Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    Box2D/Dynamics/Contacts/b2PolygonContact.h \
    Box2D/Dynamics/Joints/b2DistanceJoint.h \
    Box2D/Dynamics/Joints/b2FrictionJoint.h \
    Box2D/Dynamics/Joints/b2GearJoint.h \
    Box2D/Dynamics/Joints/b2Joint.h \
    Box2D/Dynamics/Joints/b2MotorJoint.h \
    Box2D/Dynamics/Joints/b2MouseJoint.h \
    Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    Box2D/Dynamics/Joints/b2PulleyJoint.h \
    Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    Box2D/Dynamics/Joints/b2RopeJoint.h \
    Box2D/Dynamics/Joints/b2WeldJoint.h \
    Box2D/Dynamics/Joints/b2WheelJoint.h \
    Box2D/Dynamics/b2Body.h \
    Box2D/Dynamics/b2ContactManager.h \
    Box2D/Dynamics/b2Fixture.h \
    Box2D/Dynamics/b2Island.h \
    Box2D/Dynamics/b2TimeStep.h \
    Box2D/Dynamics/b2World.h \
    Box2D/Dynamics/b2WorldCallbacks.h \
    Box2D/Particle/b2Particle.h \
    Box2D/Particle/b2ParticleAssembly.h \
    Box2D/Particle/b2ParticleGroup.h \
    Box2D/Particle/b2ParticleSystem.h \
    Box2D/Particle/b2StackQueue.h \
    Box2D/Particle/b2VoronoiDiagram.h \
    Box2D/Rope/b2Rope.h \
    applicationmodel.h \
    barmodel.h \
    drinkview.h \
    gamelayer.h \
    glassware.h \
    liquidmodel.h \
    mainmenupage.h \
    mainwindow.h \
    recipe.h \
    recipenote.h \
    settingsmenupage.h

FORMS += \
    gamelayer.ui \
    mainmenupage.ui \
    mainwindow.ui \
    settingsmenupage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gamegraphics.qrc \
    menugraphics.qrc \
    music.qrc \
    sounds.qrc \
    misc.qrc

PRECOMPILED_HEADER +=

DISTFILES += \
    Box2D/Box2D.vcxproj \
    Box2D/Box2D.vcxproj.filters \
    Box2D/Box2DConfig.cmake \
    Box2D/Box2DConfig.cmake.in \
    Box2D/CMakeLists.txt \
    Box2D/Documentation/API-Ref/doxyfile \
    Box2D/Documentation/Building/Building.md \
    Box2D/Documentation/Building/BuildingAndroid.md \
    Box2D/Documentation/Building/BuildingJavaScript.md \
    Box2D/Documentation/Building/BuildingLinux.md \
    Box2D/Documentation/Building/BuildingOSX.md \
    Box2D/Documentation/Building/BuildingWindows.md \
    Box2D/Documentation/Building/BuildingiOS.md \
    Box2D/Documentation/Building/PortingFromBox2D.md \
    Box2D/Documentation/Building/doxyfile \
    Box2D/Documentation/Programmers-Guide/Chapter01_Introduction.md \
    Box2D/Documentation/Programmers-Guide/Chapter02_Hello_Box2D.md \
    Box2D/Documentation/Programmers-Guide/Chapter03_Common.md \
    Box2D/Documentation/Programmers-Guide/Chapter04_Collision_Module.md \
    Box2D/Documentation/Programmers-Guide/Chapter05_Dynamics_Module.md \
    Box2D/Documentation/Programmers-Guide/Chapter06_Bodies.md \
    Box2D/Documentation/Programmers-Guide/Chapter07_Fixtures.md \
    Box2D/Documentation/Programmers-Guide/Chapter08_Joints.md \
    Box2D/Documentation/Programmers-Guide/Chapter09_Contacts.md \
    Box2D/Documentation/Programmers-Guide/Chapter10_World.md \
    Box2D/Documentation/Programmers-Guide/Chapter11_Particles.md \
    Box2D/Documentation/Programmers-Guide/Chapter12_Loose_Ends.md \
    Box2D/Documentation/Programmers-Guide/Chapter13_Debug_Drawing.md \
    Box2D/Documentation/Programmers-Guide/Chapter14_Limitations.md \
    Box2D/Documentation/Programmers-Guide/Chapter15_References.md \
    Box2D/Documentation/Programmers-Guide/ContentLicense.md \
    Box2D/Documentation/Programmers-Guide/Logo.md \
    Box2D/Documentation/Programmers-Guide/doxyfile \
    Box2D/Documentation/Programmers-Guide/image_0.png \
    Box2D/Documentation/Programmers-Guide/image_1.png \
    Box2D/Documentation/Programmers-Guide/image_10.png \
    Box2D/Documentation/Programmers-Guide/image_11.png \
    Box2D/Documentation/Programmers-Guide/image_12.png \
    Box2D/Documentation/Programmers-Guide/image_13.png \
    Box2D/Documentation/Programmers-Guide/image_14.png \
    Box2D/Documentation/Programmers-Guide/image_15.png \
    Box2D/Documentation/Programmers-Guide/image_16.png \
    Box2D/Documentation/Programmers-Guide/image_17.png \
    Box2D/Documentation/Programmers-Guide/image_18.gif \
    Box2D/Documentation/Programmers-Guide/image_19.gif \
    Box2D/Documentation/Programmers-Guide/image_2.gif \
    Box2D/Documentation/Programmers-Guide/image_20.gif \
    Box2D/Documentation/Programmers-Guide/image_21.gif \
    Box2D/Documentation/Programmers-Guide/image_22.gif \
    Box2D/Documentation/Programmers-Guide/image_23.png \
    Box2D/Documentation/Programmers-Guide/image_24.png \
    Box2D/Documentation/Programmers-Guide/image_3.gif \
    Box2D/Documentation/Programmers-Guide/image_4.png \
    Box2D/Documentation/Programmers-Guide/image_5.png \
    Box2D/Documentation/Programmers-Guide/image_6.png \
    Box2D/Documentation/Programmers-Guide/image_7.png \
    Box2D/Documentation/Programmers-Guide/image_8.png \
    Box2D/Documentation/Programmers-Guide/image_9.png \
    Box2D/Documentation/Programmers-Guide/liquidfun-logo-horizontal-small.png \
    Box2D/Documentation/Programmers-Guide/liquidfun-logo-small.png \
    Box2D/Documentation/Programmers-Guide/liquidfun-logo-square-small.png \
    Box2D/Documentation/Programmers-Guide/liquidfun-logo.ai \
    Box2D/Documentation/Programmers-Guide/liquidfun-logo.png \
    Box2D/Documentation/Readme/doxyfile \
    Box2D/Documentation/Readme/jquery.js \
    Box2D/Documentation/ReleaseNotes/doxyfile \
    Box2D/Documentation/ReleaseNotes/jquery.js \
    Box2D/Documentation/SWIG/GettingStarted.md \
    Box2D/Documentation/SWIG/doxyfile \
    Box2D/Documentation/SWIG/jquery.js \
    Box2D/Documentation/SWIG/liquidfunpaint.jpg \
    Box2D/Documentation/footer.html \
    Box2D/Particle/b2ParticleAssembly.neon.s \
    Box2D/UseBox2D.cmake \
    uml.qmodel
