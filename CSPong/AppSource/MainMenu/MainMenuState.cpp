//
//  MainMenuState.cpp
//  CSPong
//  Created by Scott Downie on 30/06/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <MainMenu/MainMenuState.h>

#include <Common/TransitionSystem.h>
#include <Game/GameState.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/GUI/Base.h>
#include <ChilliSource/GUI/Button.h>

namespace CSPong
{
    //------------------------------------------------------
    //------------------------------------------------------
    void MainMenuState::CreateSystems()
    {
        m_transitionSystem = CreateSystem<TransitionSystem>(1.0f, 1.0f);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void MainMenuState::OnInit()
    {
        /*
         ==============================
         Chilli Source Tour: GUI
         ==============================
         
         GUI in Chilli Source can be created in code or from .csgui files. We are currently working on a new UI system which is simpler, more extensible and has
         a dedicated WYSIWYG editor, however the underlying principles remain the same.
         
         GUI is described hierarchically with the window at the root. Changing properties on the parent will usually effect the children i.e. disabling user interaction on the root
         will make all children non-interactable. This is a good way of creating encapsulated widgets i.e. dialogue boxes, etc.
         
         GUI uses a mixture of absolute and relative co-ordinates which is why UnifiedPosition and UnifiedSize in MainMenu.csgui take 4 floats (Rx, Ry, Ax, Ay).
         Absolute co-ords are always in screen-space (i.e. pixels) with (0, 0) at the bottom left. Setting UnifiedSize = 0 0 100 200 will always give an view at
         100x200 regardless of screen resolution or orientation. This is useful for views such as virtual thumbsticks which will have been tailored to the avergae size of a human thumb.
         Relative co-ords are specified as a percentage relative to the parent view size (again from the bottom left). Setting UnifiedPosition = 0.5 0.5 0 0 will place the view at
         the centre of its parent. Setting the UnifiedSize = 1 1 0 0 will make the view the same size as its parent. Relative co-ords are useful for UI that needs to adapt based on 
         the screen resolution.
         
         Relative size and positioning on its own is not enough to build scaleable UI you need to use anchors and consider the aspect ratio of any images. CS GUI has 2 anchor properties
         
         The local anchor "LocalAlignment" which controls where the origin of the view is i.e. where it is rendered from and the "ParentalAlignment" which controls how it anchors to its parent (in the current UI system
         parental anchoring must be enabled and this overrides the position. "UnifiedParentalOffset" can be used to offset the view from the anchor).
         
         LocalAlignment = MiddleCentre
         ParentalAlignment = MiddleCentre
         
         |           |
         |    | |    |
         |           |
         
         LocalAlignment = TopLeft
         ParentalAlignment = TopLeft
         
         || |        |
         |           |
         |           |
         
         
         Most of the views that are backed by images have the ability to maintain the aspect ratio of the underlying image using various policies (see ImageView constructor for more info).
         
         ------------------------------
         
         Next: 'Events' in MainMenuState::OnInit
         */
        
        //Load the GUI and add it to the window
        CSGUI::GUIViewSPtr mainMenuView = CSGUI::GUIViewFactory::CreateGUIViewFromScript(CSCore::StorageLocation::k_package, "GUI/MainMenu.csgui");
        GetScene()->GetWindow()->AddSubview(mainMenuView);
        

        //Grab the button children by name and add this class as a listener for button events
        m_playButton = std::static_pointer_cast<CSGUI::HighlightButton>(mainMenuView->GetSubviewWithName("PlayButton"));
        //Disable user interaction until the animation is complete.
        m_playButton->EnableUserInteraction(false);
        
        //Create a tween that will slide the UI onto the screen. This will be used to offset the button from its stated position.
        m_playButtonTween = CSCore::MakeEaseInOutBackTween(-1.0f, 0.0f, 1.0f);
        
        /*
         ==============================
         Chilli Source Tour: Events
         ==============================
         
         Chilli Source has various ways to register as a listener for an action. The simplest is simply setting a callback delegate using std::function which provides a one to one mapping. One of the most common
         methods is by using "events". Events allow multiple listeners to register against the same action. Listeners open a scoped connection to the event and while the connection renamins open will
         be notifed when the event occurs. Connections can be closed by either end (usually when they go out of scope).
         
         Buttons use events to inform listeners when they have been activated. The delegate that are passed to events can be std::functions or in this case an anonymous lambda function:
         
         ------------------------------
         
         Next: 'Entities' in GameState::OnInit
         */
        m_playButtonConnection = m_playButton->GetActivatedEvent().OpenConnection([this](CSGUI::Button*)
        {
            m_playButton->EnableUserInteraction(false);
            m_playButtonTween.Play(CSCore::TweenPlayMode::k_onceReverse);
            m_playButtonTween.SetOnEndDelegate([this](CSCore::EaseInOutBackTween<f32>* in_tween)
            {
                m_transitionSystem->Transition(CSCore::StateSPtr(new GameState()));
            });
        });
        
        m_transitionInConnection = m_transitionSystem->GetTransitionInFinishedEvent().OpenConnection([this]()
        {
            m_playButtonTween.Play(CSCore::TweenPlayMode::k_once);
            
            //Once the animation is finished allow the user to press the button
            m_playButtonTween.SetOnEndDelegate([this](CSCore::EaseInOutBackTween<f32>* in_tween)
            {
                m_playButton->EnableUserInteraction(true);
            });
        });
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void MainMenuState::OnUpdate(f32 dt)
    {
        m_playButtonTween.Update(dt);
        m_playButton->SetOffsetFromPosition(m_playButtonTween.GetValue(), 0.0f, 0.0f, 0.0f);
    }
}

