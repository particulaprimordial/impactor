#include "PluginEditor.h"

ParticulaImpactEngineAudioProcessorEditor::ParticulaImpactEngineAudioProcessorEditor(
    ParticulaImpactEngineAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(720, 430);
    addControl("startFreq", "START FREQ");
    addControl("endFreq", "END FREQ");
    addControl("pitchDecay", "PITCH DECAY");
    addControl("ampDecay", "AMP DECAY");
    addControl("harmonics", "HARMONICS");
    addControl("inharmonicity", "INHARMONICITY");
    addControl("noise", "NOISE");
    addControl("click", "TRANSIENT");
    addControl("drive", "DRIVE");
}

void ParticulaImpactEngineAudioProcessorEditor::addControl(const juce::String& id,
                                                            const juce::String& text)
{
    auto c = std::make_unique<Control>();
    c->label.setText(text, juce::dontSendNotification);
    c->label.setJustificationType(juce::Justification::centred);
    c->label.setColour(juce::Label::textColourId, juce::Colour(0xffb8c2cc));
    c->slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    c->slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 74, 20);
    c->slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffd2ad68));
    c->slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xfff0d7a0));
    c->attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.parameters, id, c->slider);
    addAndMakeVisible(c->label);
    addAndMakeVisible(c->slider);
    controls.push_back(std::move(c));
}

void ParticulaImpactEngineAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff10151b));
    g.setColour(juce::Colour(0xffd2ad68));
    g.setFont(juce::Font(22.0f, juce::Font::bold));
    g.drawText("PARTICULA PRIMORDIAL", 24, 18, 420, 28, juce::Justification::left);
    g.setColour(juce::Colour(0xff9ba8b5));
    g.setFont(11.0f);
    g.drawText("IMPACT ENGINE  /  PROCEDURAL PERCUSSION SYNTHESIZER  /  V0.1",
               25, 48, 600, 18, juce::Justification::left);
    g.setColour(juce::Colour(0xff303b46));
    g.drawHorizontalLine(78, 24.0f, 696.0f);
}

void ParticulaImpactEngineAudioProcessorEditor::resized()
{
    const int margin = 22;
    const int top = 92;
    const int cellW = (getWidth() - margin * 2) / 5;
    const int cellH = 158;

    for (size_t i = 0; i < controls.size(); ++i)
    {
        const int col = (int) (i % 5);
        const int row = (int) (i / 5);
        const int x = margin + col * cellW;
        const int y = top + row * cellH;
        controls[i]->label.setBounds(x, y, cellW, 22);
        controls[i]->slider.setBounds(x + 8, y + 24, cellW - 16, 112);
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParticulaImpactEngineAudioProcessor();
}
