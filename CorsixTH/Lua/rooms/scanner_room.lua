--[[ Copyright (c) 2009 Manuel König

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. --]]

local room = {}
room.id = "scanner"
room.vip_must_visit = false
room.level_config_id = 13
room.class = "ScannerRoom"
room.name = _S.rooms_short.scanner
room.long_name = _S.rooms_long.scanner
room.tooltip = _S.tooltip.rooms.scanner
room.objects_additional = { "extinguisher", "radiator", "plant", "bin" }
room.objects_needed = { scanner = 1, console = 1, screen = 1 }
room.build_preview_animation = 920
room.categories = {
  diagnosis = 4,
}
room.minimum_size = 5
room.wall_type = "yellow"
room.floor_tile = 19
room.required_staff = {
  Doctor = 1,
}
room.maximum_staff = room.required_staff
room.call_sound = "reqd002.wav"
-- Handyman is called to "diagnosis machine", all other diagnosis rooms have
-- their own, more specific handyman call though
--room.handyman_call_sound = "maint011.wav"

class "ScannerRoom" (Room)

---@type ScannerRoom
local ScannerRoom = _G["ScannerRoom"]

function ScannerRoom:ScannerRoom(...)
  self:Room(...)
end

function ScannerRoom:commandEnteringPatient(patient)
  local staff = self.staff_member
  local console, stf_x, stf_y = self.world:findObjectNear(staff, "console")
  local scanner, pat_x, pat_y = self.world:findObjectNear(patient, "scanner")
  local screen, sx, sy = self.world:findObjectNear(patient, "screen")
  local do_change = (patient.humanoid_class == "Standard Male Patient") or
    (patient.humanoid_class == "Standard Female Patient")

  local --[[persistable:scanner_shared_loop_callback]] function loop_callback()
    if staff:getCurrentAction().scanner_ready and patient:getCurrentAction().scanner_ready then
      staff:finishAction()
      patient:finishAction()
    end
  end

  staff:walkTo(stf_x, stf_y)
  local idle_action = IdleAction():setDirection(console.direction == "north" and "west" or "north")
      :setLoopCallback(loop_callback)
  idle_action.scanner_ready = true
  staff:queueAction(idle_action)

  staff:queueAction(UseObjectAction(console))

  if do_change then
    patient:walkTo(sx, sy)
    patient:queueAction(UseScreenAction(screen))
    patient:queueAction(WalkAction(pat_x, pat_y))
  else
    patient:walkTo(pat_x, pat_y)
  end

  idle_action = IdleAction():setDirection(scanner.direction == "north" and "east" or "south")
      :setLoopCallback(loop_callback)
  idle_action.scanner_ready = true
  patient:queueAction(idle_action)

  local length = math.random(10, 20) * (2 - staff.profile.skill)
  local loop_callback_scan = --[[persistable:scanner_loop_callback]] function(action)
    if length <= 0 then
      action.prolonged_usage = false
    end
    length = length - 1
  end

  local after_use_scan = --[[persistable:scanner_after_use]] function()
    if not self.staff_member or patient.going_home then
      -- If we aborted somehow, don't do anything here.
      -- The patient already has orders to change back if necessary and leave.
      -- makeHumanoidLeave() will make this function nil when it aborts the scanner's use.
      return
    end
    self.staff_member:setNextAction(MeanderAction())
    self:dealtWithPatient(patient)
  end

  patient:queueAction(UseObjectAction(scanner):setLoopCallback(loop_callback_scan)
      :setAfterUse(after_use_scan))
  return Room.commandEnteringPatient(self, patient)
end

function ScannerRoom:onHumanoidLeave(humanoid)
  if self.staff_member == humanoid then
    self.staff_member = nil
  end
  Room.onHumanoidLeave(self, humanoid)
end

function ScannerRoom:makeHumanoidLeave(humanoid)
  if humanoid:getCurrentAction().name == "use_object" and
      humanoid:getCurrentAction().object == self.world:findObjectNear(humanoid, "scanner") then
    humanoid:getCurrentAction().after_use = nil
  end

  self:makeHumanoidDressIfNecessaryAndThenLeave(humanoid)
end

function ScannerRoom:dealtWithPatient(patient)
  if string.find(patient.humanoid_class, "Stripped") then
    local screen, sx, sy = self.world:findObjectNear(patient, "screen")
    patient:setNextAction(WalkAction(sx, sy):setMustHappen(true):setIsLeaving(true)
        :disableTruncate())

    local after_use_patient = --[[persistable:scanner_exit]] function()
      Room.dealtWithPatient(self, patient)
    end

    patient:queueAction(UseScreenAction(screen):setMustHappen(true):setIsLeaving(true)
        :setAfterUse(after_use_patient))
    patient:queueAction(self:createLeaveAction())
  else
    Room.dealtWithPatient(self, patient)
  end
end

function ScannerRoom:shouldHavePatientReenter(patient)
  return not patient:isLeaving()
end

return room
