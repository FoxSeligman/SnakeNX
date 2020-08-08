#pragma once

#include <switch.h>

namespace atmy {

class Controller {
 public:
  virtual void OnButtonsDown(u64 buttons) {}
  virtual void OnButtonsHeld() {}
  virtual void OnButtonsUp() {}
  virtual void OnAxis() {}
  virtual void OnHatChange() {}
  virtual void OnCaptureButton() {}

  virtual void OnAccelerometer() {}   // must be enabled first

  virtual void OnFingerDown() {}      // must be enabled first
  virtual void OnFingerMove() {}
  virtual void OnFingerUp() {}
};

}  // namespace atmy
