#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "CrusherDSP.h"

const int kNumPresets = 10;

enum EParams
{
  kGain = 0,
  kCrushMix,
  kCrushBitRate,
  kCrushDownSample,
  kTest,
  kNumParams
};

enum EControlTags
{
  kTagTest
};

using namespace iplug;
using namespace igraphics;

class BitCrushi final : public Plugin
{
public:
  BitCrushi(const InstanceInfo& info);
  CrusherDSP crush;
  double sampleL_POST;
  double sampleR_POST;

private:
  

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
#endif
};
