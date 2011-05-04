#include "enums.hpp"

namespace Enum {
StaysOnTop::Map StaysOnTop::_map;
const StaysOnTop StaysOnTop::Always(0, "Always");
const StaysOnTop StaysOnTop::Playing(0+1, "Playing");
const StaysOnTop StaysOnTop::Never(0+1+1, "Never");
}

namespace Enum {
SeekingStep::Map SeekingStep::_map;
const SeekingStep SeekingStep::Step1(0, "Step1");
const SeekingStep SeekingStep::Step2(0+1, "Step2");
const SeekingStep SeekingStep::Step3(0+1+1, "Step3");
}

namespace Enum {
GeneratePlaylist::Map GeneratePlaylist::_map;
const GeneratePlaylist GeneratePlaylist::Similar(0, "Similar");
const GeneratePlaylist GeneratePlaylist::Folder(0+1, "Folder");
const GeneratePlaylist GeneratePlaylist::None(0+1+1, "None");
}

namespace Enum {
SubtitleAutoload::Map SubtitleAutoload::_map;
const SubtitleAutoload SubtitleAutoload::Matched(0, "Matched");
const SubtitleAutoload SubtitleAutoload::Contain(0+1, "Contain");
const SubtitleAutoload SubtitleAutoload::Folder(0+1+1, "Folder");
const SubtitleAutoload SubtitleAutoload::None(0+1+1+1, "None");
}

namespace Enum {
SubtitleAutoselect::Map SubtitleAutoselect::_map;
const SubtitleAutoselect SubtitleAutoselect::Matched(0, "Matched");
const SubtitleAutoselect SubtitleAutoselect::First(0+1, "First");
const SubtitleAutoselect SubtitleAutoselect::All(0+1+1, "All");
const SubtitleAutoselect SubtitleAutoselect::EachLanguage(0+1+1+1, "EachLanguage");
}

namespace Enum {
OsdAutoSize::Map OsdAutoSize::_map;
const OsdAutoSize OsdAutoSize::Width(0, "Width");
const OsdAutoSize OsdAutoSize::Height(0+1, "Height");
const OsdAutoSize OsdAutoSize::Diagonal(0+1+1, "Diagonal");
}

namespace Enum {
ClickAction::Map ClickAction::_map;
const ClickAction ClickAction::OpenFile(0, "OpenFile");
const ClickAction ClickAction::Fullscreen(0+1, "Fullscreen");
const ClickAction ClickAction::Pause(0+1+1, "Pause");
const ClickAction ClickAction::Mute(0+1+1+1, "Mute");
}

namespace Enum {
WheelAction::Map WheelAction::_map;
const WheelAction WheelAction::Seek1(0, "Seek1");
const WheelAction WheelAction::Seek2(0+1, "Seek2");
const WheelAction WheelAction::Seek3(0+1+1, "Seek3");
const WheelAction WheelAction::PrevNext(0+1+1+1, "PrevNext");
const WheelAction WheelAction::Volume(0+1+1+1+1, "Volume");
const WheelAction WheelAction::Amp(0+1+1+1+1+1, "Amp");
}

namespace Enum {
KeyModifier::Map KeyModifier::_map;
const KeyModifier KeyModifier::None(Qt::NoModifier, "None");
const KeyModifier KeyModifier::Ctrl(Qt::ControlModifier, "Ctrl");
const KeyModifier KeyModifier::Shift(Qt::ShiftModifier, "Shift");
const KeyModifier KeyModifier::Alt(Qt::AltModifier, "Alt");
}

