/* AllGears.h
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "gears/Gear_AudioInput.h"
#include "gears/Gear_AudioOutput.h"

#include "gears/Gear_STKAudioOutput.h"
#include "GearMaker.h"
#include "gears/Gear_AudioSource.h"
#include "gears/Gear_AverageSignal.h"
#include "gears/Gear_Blur.h"
#include "gears/Gear_ColorAdjust.h"
#include "gears/Gear_Contrast.h"
#include "gears/Gear_ColorQuantize.h"
#include "gears/Gear_ClusteredDither.h"
#include "gears/Gear_DataOut.h"
#include "gears/Gear_DiffDist.h"
#include "gears/Gear_Edge.h"
#include "gears/Gear_Enveloppe.h"

//#include "src/gears/Gear_Enveloppe.h"
//#include "gears/Gear_FilterSignal.h"
#include "gears/Gear_FreqBoxes.h"
#include "gears/Gear_Gain.h"
#include "gears/Gear_GrayScale.h"
#include "gears/Gear_HalfToning.h"
#include "gears/Gear_Image.h"
#include "gears/Gear_KDTree.h"
#include "gears/Gear_MasterFrequency.h"
#include "gears/Gear_MedianFilter.h"
#include "gears/Gear_NGone.h"
#include "gears/Gear_Osc.h"
#include "gears/Gear_Oscilloscope.h"
#include "gears/Gear_PatternGen.h"
#include "gears/Gear_Saturation.h"
#include "gears/Gear_SimpleDelay.h"
#include "gears/Gear_Slider.h"
#include "gears/Gear_TV.h"
#include "gears/Gear_VideoAdd.h"
#include "gears/Gear_VideoBlend.h"
#include "gears/Gear_VideoInput.h"
#include "gears/Gear_VideoMix.h"
#include "gears/Gear_VideoOutput.h"
#include "gears/Gear_VideoSource.h"

#include "VideoOutputX11Base.h"
#include "VideoOutputXv.h"
#include "VideoOutputShm.h"
#include "VideoOutputGl.h"


