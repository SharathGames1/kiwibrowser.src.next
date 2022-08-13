// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdlib.h>

#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "content/public/test/content_browser_test.h"
#include "content/public/test/content_browser_test_utils.h"
#include "content/public/test/test_navigation_observer.h"
#include "content/shell/browser/shell.h"

namespace content {

class ResourceLoadingBrowserTest : public ContentBrowserTest  {
};

const char kResourceLoadingNonMobilePage[] =
    "/resource_loading/resource_loading_non_mobile.html";

// crbug.com/1339969
IN_PROC_BROWSER_TEST_F(ResourceLoadingBrowserTest,
                       ResourceLoadingAvoidDoubleDownloads) {
  ASSERT_TRUE(embedded_test_server()->Start());
  GURL url = embedded_test_server()->GetURL(kResourceLoadingNonMobilePage);
  EXPECT_TRUE(NavigateToURL(shell(), url));
  int resoureceNumber =
      EvalJs(shell(), "getResourceNumber()", EXECUTE_SCRIPT_USE_MANUAL_REPLY)
          .ExtractInt();
  // Hacky way to get the flaky extra resource timing entry content to logs.
  if (resoureceNumber != 9) {
    EXPECT_EQ("", EvalJs(shell(), "getResources()"));
  }
  EXPECT_EQ(9, resoureceNumber);
}

} // namespace content
