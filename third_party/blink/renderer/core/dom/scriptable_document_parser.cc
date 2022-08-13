/*
 * Copyright (C) 2010 Google, Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/dom/scriptable_document_parser.h"

#include "third_party/blink/renderer/bindings/core/v8/script_streamer.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/platform/loader/fetch/source_keyed_cached_metadata_handler.h"

namespace blink {

ScriptableDocumentParser::ScriptableDocumentParser(
    Document& document,
    ParserContentPolicy parser_content_policy)
    : DecodedDataDocumentParser(document),
      was_created_by_script_(false),
      parser_content_policy_(parser_content_policy) {}

bool ScriptableDocumentParser::IsParsingAtLineNumber() const {
  return IsParsing() && !IsWaitingForScripts() && !IsExecutingScript();
}

void ScriptableDocumentParser::Trace(Visitor* visitor) const {
  visitor->Trace(inline_script_cache_handler_);
  DecodedDataDocumentParser::Trace(visitor);
}

void ScriptableDocumentParser::AddInlineScriptStreamer(
    const String& source,
    scoped_refptr<BackgroundInlineScriptStreamer> streamer) {
  base::AutoLock lock(streamers_lock_);
  inline_script_streamers_.insert(source, std::move(streamer));
}

InlineScriptStreamer* ScriptableDocumentParser::TakeInlineScriptStreamer(
    const String& source) {
  scoped_refptr<BackgroundInlineScriptStreamer> streamer;
  {
    base::AutoLock lock(streamers_lock_);
    streamer = inline_script_streamers_.Take(source);
  }
  // If the streamer hasn't started yet, cancel and just compile on the main
  // thread.
  if (streamer && !streamer->IsStarted()) {
    streamer->Cancel();
    streamer = nullptr;
  }
  if (streamer)
    return InlineScriptStreamer::From(std::move(streamer));
  return nullptr;
}

void ScriptableDocumentParser::AddCSSTokenizer(
    const String& source,
    std::unique_ptr<CSSTokenizerBase> tokenizer) {
  base::AutoLock lock(tokenizers_lock_);
  inline_css_tokenizers_.insert(source, std::move(tokenizer));
}

std::unique_ptr<CSSTokenizerBase> ScriptableDocumentParser::TakeCSSTokenizer(
    const String& source) {
  base::AutoLock lock(tokenizers_lock_);
  return inline_css_tokenizers_.Take(source);
}

}  // namespace blink
