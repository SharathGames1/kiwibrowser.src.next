// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_MEDIA_VALUES_DYNAMIC_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_MEDIA_VALUES_DYNAMIC_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/css/media_values.h"

namespace blink {

class Document;

class CORE_EXPORT MediaValuesDynamic : public MediaValues {
 public:
  static MediaValues* Create(Document&);
  static MediaValues* Create(LocalFrame*);

  explicit MediaValuesDynamic(LocalFrame*);
  MediaValuesDynamic(LocalFrame*,
                     bool overridden_viewport_dimensions,
                     double viewport_width,
                     double viewport_height);

  int DeviceWidth() const override;
  int DeviceHeight() const override;
  float DevicePixelRatio() const override;
  bool DeviceSupportsHDR() const override;
  int ColorBitsPerComponent() const override;
  int MonochromeBitsPerComponent() const override;
  mojom::blink::PointerType PrimaryPointerType() const override;
  int AvailablePointerTypes() const override;
  mojom::blink::HoverType PrimaryHoverType() const override;
  int AvailableHoverTypes() const override;
  bool ThreeDEnabled() const override;
  bool InImmersiveMode() const override;
  bool StrictMode() const override;
  const String MediaType() const override;
  blink::mojom::DisplayMode DisplayMode() const override;
  ColorSpaceGamut ColorGamut() const override;
  mojom::blink::PreferredColorScheme GetPreferredColorScheme() const override;
  mojom::blink::PreferredContrast GetPreferredContrast() const override;
  bool PrefersReducedMotion() const override;
  bool PrefersReducedData() const override;
  ForcedColors GetForcedColors() const override;
  NavigationControls GetNavigationControls() const override;
  int GetHorizontalViewportSegments() const override;
  int GetVerticalViewportSegments() const override;
  device::mojom::blink::DevicePostureType GetDevicePosture() const override;
  Document* GetDocument() const override;
  bool HasValues() const override;

  void Trace(Visitor*) const override;

 protected:
  // CSSLengthResolver
  float EmFontSize() const override;
  float RemFontSize() const override;
  float ExFontSize() const override;
  float ChFontSize() const override;
  float IcFontSize() const override;
  double ViewportWidth() const override;
  double ViewportHeight() const override;
  double SmallViewportWidth() const override;
  double SmallViewportHeight() const override;
  double LargeViewportWidth() const override;
  double LargeViewportHeight() const override;
  double DynamicViewportWidth() const override;
  double DynamicViewportHeight() const override;
  double ContainerWidth() const override;
  double ContainerHeight() const override;
  WritingMode GetWritingMode() const override {
    return WritingMode::kHorizontalTb;
  }

  Member<LocalFrame> frame_;
  bool viewport_dimensions_overridden_;
  double viewport_width_override_;
  double viewport_height_override_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_MEDIA_VALUES_DYNAMIC_H_
