#include "BitCrushi.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"


BitCrushi::BitCrushi(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kGain)->InitDouble("Gain", 100.0, 0., 100.0, 1, "%");
  GetParam(kCrushMix)->InitDouble("Dry/Wet", 100, 0., 100.0, 1, "%");
  GetParam(kCrushBitRate)->InitDouble("BitRate", 16, 4, 16, 2, "Rate");
  GetParam(kCrushDownSample)->InitDouble("Downsampling", 16, 2, 32, 2, "Rate");


#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };


  mLayoutFunc = [&](IGraphics* pGraphics) {

    /*
    bg.png
    bg_low_res.png
    Parameters
    knob_gain.png
    knob_crushmix.png
    knob_bitrate.PNG
    knob_crushdown.PNG
    */

    const IBitmap bitmapBG = pGraphics->LoadBitmap(BG_FN, 1/* num frames*/);
    const IBitmap bitmapKNOB = pGraphics->LoadBitmap(KNOB_FN, 101/* num frames*/);

    const IBitmap bitmapBG_LOWRES = pGraphics->LoadBitmap(BG_FN_LOW_RES, 1/* num frames*/);
    const IBitmap bitmapKNOB_BITRATE = pGraphics->LoadBitmap(KNOB_FN_BITRATE, 101/* num frames*/);
    const IBitmap bitmapKNOB_CRUSHDOWN = pGraphics->LoadBitmap(KNOB_FN_CRUSHDOWN, 101/* num frames*/);
    const IBitmap bitmapKNOB_CRUSHMIX = pGraphics->LoadBitmap(KNOB_FN_GAIN, 101/* num frames*/);
    const IBitmap bitmapKNOB_GAIN = pGraphics->LoadBitmap(KNOB_FN_GAIN, 101/* num frames*/);

    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachBackground(BG_FN);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    const IVStyle style
    {
       true, // Show label
       true, // Show value
    {

    },
    IText(25.f, EAlign::Near,COLOR_WHITE) // Label text
    };

    
    const IRECT b = pGraphics->GetBounds();
    const int nRows = 4;
    const int nCols = 2;

    int cellIdx = -1;

    auto sameCell = [&]() {
      return b.GetGridCell(cellIdx, nRows, nCols).GetPadded(-5.);
    };

    auto nextCell = [&]() {
      return b.GetGridCell(++cellIdx, nRows, nCols).GetPadded(-40);
    };

   
    pGraphics->AttachControl(new ITextControl(nextCell(), " ", DEFAULT_TEXT, DEFAULT_BGCOLOR, false));
    pGraphics->AttachControl(new ITextControl(nextCell(), " ", DEFAULT_TEXT, DEFAULT_BGCOLOR, false));
    //header fo the badge in the background image

    //FUTURE RELEASE
    /*pGraphics->AttachControl(new ITextControl(nextCell(), "BitRate", style.labelText));
    pGraphics->AttachControl(new IBKnobControl(nextCell(), bitmapKNOB_BITRATE, kCrushBitRate), kNoTag, "BitRate");*/

    pGraphics->AttachControl(new ITextControl(nextCell(), "Sample Rate", style.labelText));
    pGraphics->AttachControl(new IBKnobControl(nextCell(), bitmapKNOB_BITRATE, kCrushDownSample), kNoTag, "BitRate");

    pGraphics->AttachControl(new ITextControl(nextCell(), "Dry / Wet", style.labelText));
    pGraphics->AttachControl(new IBKnobControl(nextCell(), bitmapKNOB_CRUSHMIX, kCrushMix), kNoTag, "Dry /Wet ");

    pGraphics->AttachControl(new ITextControl(nextCell(), "Master Vol", style.labelText));
    pGraphics->AttachControl(new IBKnobControl(nextCell(), bitmapKNOB_GAIN, kGain), kNoTag, "BitRate");


  };
#endif
}

#if IPLUG_DSP
void BitCrushi::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const double gain = GetParam(kGain)->Value() / 100.;
  const double crushMix = GetParam(kCrushMix)->Value() / 100.;
  const int crushBitRate = GetParam(kCrushBitRate)->Value();
  const int crushDSRate = GetParam(kCrushDownSample)->Value();
  const int nChans = NOutChansConnected();
  double gain_crushMix;
  gain_crushMix = 1 - crushMix;

  for (int s = 0; s < nFrames; s++) {
    for (int c = 0; c < nChans; c++) {

      double sampleL_PRE = inputs[0][s];
      double sampleR_PRE = inputs[1][s];
      if (s % crushDSRate == 0)
      {
        sampleL_POST = sampleL_PRE;
        sampleR_POST = sampleR_PRE;
      }

      outputs[0][s] = ((sampleL_POST * crushMix) + (sampleL_PRE * gain_crushMix)) * gain;
      outputs[1][s] = ((sampleR_POST * crushMix) + (sampleR_PRE * gain_crushMix)) * gain;

    }
  }
}
#endif
