#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

namespace
{
    constexpr double twoPi = juce::MathConstants<double>::twoPi;
    inline float clamp01(float x) { return juce::jlimit(0.0f, 1.0f, x); }
}

ParticulaImpactEngineAudioProcessor::ParticulaImpactEngineAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
ParticulaImpactEngineAudioProcessor::createParameterLayout()
{
    using APF = juce::AudioParameterFloat;
    using R = juce::NormalisableRange<float>;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;

    p.push_back(std::make_unique<APF>("startFreq", "Start Frequency", R(25.0f, 1200.0f, 0.1f, 0.35f), 180.0f));
    p.push_back(std::make_unique<APF>("endFreq", "End Frequency", R(25.0f, 220.0f, 0.1f, 0.45f), 48.0f));
    p.push_back(std::make_unique<APF>("pitchDecay", "Pitch Decay", R(5.0f, 500.0f, 0.1f, 0.5f), 45.0f));
    p.push_back(std::make_unique<APF>("ampDecay", "Amplitude Decay", R(40.0f, 2500.0f, 1.0f, 0.5f), 420.0f));
    p.push_back(std::make_unique<APF>("harmonics", "Harmonics", R(1.0f, 12.0f, 1.0f), 3.0f));
    p.push_back(std::make_unique<APF>("inharmonicity", "Inharmonicity", R(0.0f, 1.0f, 0.001f), 0.0f));
    p.push_back(std::make_unique<APF>("noise", "Noise", R(0.0f, 1.0f, 0.001f), 0.025f));
    p.push_back(std::make_unique<APF>("click", "Transient", R(0.0f, 1.0f, 0.001f), 0.12f));
    p.push_back(std::make_unique<APF>("drive", "Drive", R(0.0f, 12.0f, 0.01f), 1.5f));
    return { p.begin(), p.end() };
}

float ParticulaImpactEngineAudioProcessor::readParam(const char* id) const
{
    return parameters.getRawParameterValue(id)->load();
}

void ParticulaImpactEngineAudioProcessor::prepareToPlay(double sampleRate, int)
{
    sr = sampleRate;
    phase = subPhase = 0.0;
    active = false;
}

void ParticulaImpactEngineAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                        juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            currentVelocity = msg.getFloatVelocity();
            phase = subPhase = 0.0;
            pitchAge = ampAge = 0.0;
            active = true;
        }
        else if (msg.isAllNotesOff() || msg.isAllSoundOff())
        {
            active = false;
        }
    }

    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        const float s = active ? renderSample() : 0.0f;
        left[i] = s;
        if (right != nullptr)
            right[i] = s;
    }
}

float ParticulaImpactEngineAudioProcessor::renderSample()
{
    const double pitchMs = juce::jmax(1.0f, readParam("pitchDecay"));
    const double ampMs = juce::jmax(1.0f, readParam("ampDecay"));
    const double p = std::exp(-pitchAge / (pitchMs * 0.001 * sr));
    const double a = std::exp(-ampAge / (ampMs * 0.001 * sr));

    const double f0 = readParam("endFreq")
        + (readParam("startFreq") - readParam("endFreq")) * p;

    const int count = juce::jlimit(1, 12, (int) std::round(readParam("harmonics")));
    const double inharm = readParam("inharmonicity");
    double sum = 0.0;
    double norm = 0.0;

    for (int h = 1; h <= count; ++h)
    {
        const double n = (double) h;
        const double ratio = n * std::sqrt(1.0 + inharm * (n * n - 1.0));
        const double weight = 1.0 / std::pow(n, 1.15);
        sum += weight * std::sin(phase * ratio);
        norm += weight;
    }

    const double clickEnv = std::exp(-ampAge / (0.0025 * sr));
    const double click = readParam("click") * clickEnv
        * (random.nextFloat() * 2.0f - 1.0f) * 0.22;

    const double noiseEnv = std::exp(-ampAge / (0.025 * sr));
    const double noise = readParam("noise") * noiseEnv
        * (random.nextFloat() * 2.0f - 1.0f);

    const double mono = (sum / juce::jmax(0.0001, norm)) * a;
    const double drive = 1.0 + readParam("drive");

    const float out = (float) (std::tanh((mono + click + noise) * drive) * currentVelocity);

    phase += twoPi * f0 / sr;
    if (phase > twoPi * 100000.0)
        phase = std::fmod(phase, twoPi);

    pitchAge += 1.0;
    ampAge += 1.0;

    if (a < 0.00008)
        active = false;

    return out;
}

juce::AudioProcessorEditor* ParticulaImpactEngineAudioProcessor::createEditor()
{
    return new ParticulaImpactEngineAudioProcessorEditor(*this);
}

void ParticulaImpactEngineAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ParticulaImpactEngineAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}
