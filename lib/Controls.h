#ifndef CONTROLS_H
#define CONTROLS_H

class Controls{

	gf::Action closeWindowAction;
    gf::Action leftAction;
    gf::Action rightAction;
    gf::Action rotateAction;
    gf::Action downAction;

    gf::ActionContainer actions;
    
	public:
		Controls() : 
			closeWindowAction("Close"),
			leftAction("Left"),
			rightAction("Right"),
			rotateAction("Rotate"),
			downAction("Down"){

			closeWindowAction.addCloseControl();
		    closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
		    
			leftAction.addScancodeKeyControl(gf::Scancode::Q);
		    leftAction.addScancodeKeyControl(gf::Scancode::Left);
		    leftAction.setInstantaneous();

			rightAction.addScancodeKeyControl(gf::Scancode::D);
		    rightAction.addScancodeKeyControl(gf::Scancode::Right);
		    rightAction.setInstantaneous();

			rotateAction.addScancodeKeyControl(gf::Scancode::Space);
		    rotateAction.setInstantaneous();

			downAction.addScancodeKeyControl(gf::Scancode::S);
		    downAction.addScancodeKeyControl(gf::Scancode::Down);
		    downAction.setContinuous();

		    actions.addAction(closeWindowAction);
			actions.addAction(leftAction);
			actions.addAction(rightAction);
			actions.addAction(rotateAction);
			actions.addAction(downAction);
		}

		gf::Action & operator()(std::string s){
			return actions.getAction(s);
		}

		void reset(){
			actions.reset();
		}

		void processEvent(gf::Event & event){
			actions.processEvent(event);
		}
	
	
};

#endif // CONTROLS_H