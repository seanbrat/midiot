//
//  GraphicsComponentBase.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/14/17.
//
//

#ifndef GraphicsComponentBase_hpp
#define GraphicsComponentBase_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

#include "DemoUtilities.h"



//==============================================================================
class GraphicsComponentBase  : public Component
{
public:
    GraphicsComponentBase (const String& name)
    : Component (name)
    {
        displayFont = Font (Font::getDefaultMonospacedFontName(), 12.0f, Font::bold);
    }
    
    AffineTransform getTransform()
    {
        const float hw = 0.5f * getWidth();
        const float hh = 0.5f * getHeight();
        
        AffineTransform t;
        
        //if (controls.animateRotation.getToggleState())
        //    t = t.rotated (rotation.getValue() * float_Pi * 2.0f);
        
        //if (controls.animateSize.getToggleState())
        //    t = t.scaled (0.3f + size.getValue() * 2.0f);
        
        //if (controls.animatePosition.getToggleState())
        //    t = t.translated (hw + hw * (offsetX.getValue() - 0.5f),
        //                      hh + hh * (offsetY.getValue() - 0.5f));
        //else
        t = t.translated (hw, hh);
        
        //if (controls.animateShear.getToggleState())
        //    t = t.sheared (shear.getValue() * 2.0f - 1.0f, 0.0f);
        
        return t;
    }
    
    float getAlpha() const
    {
        //if (controls.animateAlpha.getToggleState())
        return alpha.getValue();
        
        return 1.0f;
    }
    
    
    
    void paint (Graphics& g) override
    {
        double startTime = 0.0;
        
        {
            // A ScopedSaveState will return the Graphics context to the state it was at the time of
            // construction when it goes out of scope. We use it here to avoid clipping the fps text
            const Graphics::ScopedSaveState state (g);
            
            //if (controls.clipToRectangle.getToggleState())
            //    clipToRectangle (g);
            //if (controls.clipToPath     .getToggleState())
            //    clipToPath (g);
            //if (controls.clipToImage    .getToggleState())
            //    clipToImage (g);
            
            //g.setImageResamplingQuality (controls.quality.getToggleState() ? Graphics::highResamplingQuality
            //                                         : Graphics::mediumResamplingQuality);
            //g.setImageResamplingQuality (Graphics::highResamplingQuality);
            
            // take a note of the time before the render
            //startTime = Time::getMillisecondCounterHiRes();
            
            // then let the demo draw itself..
            drawComponent (g);
        }
    }
    
    virtual void drawComponent (Graphics&) = 0;
    
    void clipToRectangle (Graphics& g)
    {
        int w = getWidth() / 2;
        int h = getHeight() / 2;
        
        int x = (int) (w * clipRectX.getValue());
        int y = (int) (h * clipRectY.getValue());
        
        g.reduceClipRegion (x, y, w, h);
    }
    
    void clipToPath (Graphics& g)
    {
        float pathSize = (float) jmin (getWidth(), getHeight());
        
        Path p;
        p.addStar (Point<float> (clipPathX.getValue(),
                                 clipPathY.getValue()) * pathSize,
                   7,
                   pathSize * (0.5f + clipPathDepth.getValue()),
                   pathSize * 0.5f,
                   clipPathAngle.getValue());
        
        g.reduceClipRegion (p, AffineTransform());
    }
    
    void clipToImage (Graphics& g)
    {
        if (! clipImage.isValid())
            createClipImage();
        
        AffineTransform transform (AffineTransform::translation (clipImage.getWidth() / -2.0f,
                                                                 clipImage.getHeight() / -2.0f)
                                   .rotated (clipImageAngle.getValue() * float_Pi * 2.0f)
                                   .scaled (2.0f + clipImageSize.getValue() * 3.0f)
                                   .translated (getWidth() * 0.5f,
                                                getHeight() * 0.5f));
        
        g.reduceClipRegion (clipImage, transform);
    }
    
    void createClipImage()
    {
        clipImage = Image (Image::ARGB, 300, 300, true);
        
        Graphics g (clipImage);
        
        g.setGradientFill (ColourGradient (Colours::transparentBlack, 0, 0,
                                           Colours::black, 0, 300, false));
        
        for (int i = 0; i < 20; ++i)
            g.fillRect (Random::getSystemRandom().nextInt (200),
                        Random::getSystemRandom().nextInt (200),
                        Random::getSystemRandom().nextInt (100),
                        Random::getSystemRandom().nextInt (100));
    }
    
    //==============================================================================
    
    SlowerBouncingNumber offsetX, offsetY, rotation, size, shear, alpha, clipRectX,
    clipRectY, clipPathX, clipPathY, clipPathDepth, clipPathAngle,
    clipImageX, clipImageY, clipImageAngle, clipImageSize;
    
    double lastRenderStartTime, averageTimeMs, averageActualFPS;
    Image clipImage;
    Font displayFont;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicsComponentBase)
};




#endif /* GraphicsComponentBase_hpp */
