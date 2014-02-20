/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 et tw=78: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/AudioTrack.h"
#include "mozilla/dom/AudioTrackBinding.h"
#include "mozilla/dom/AudioTrackList.h"
#include "mozilla/dom/HTMLMediaElement.h"
#include "mozilla/dom/HTMLTrackElement.h"

namespace mozilla {
namespace dom {

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_2(AudioTrack, mParent, mAudioTrackList)
NS_IMPL_CYCLE_COLLECTING_ADDREF(AudioTrack)
NS_IMPL_CYCLE_COLLECTING_RELEASE(AudioTrack)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(AudioTrack)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

AudioTrack::AudioTrack(nsISupports* aParent)
  : mParent(aParent)
{
  SetDefaultSettings();
  SetIsDOMBinding();
}

AudioTrack::AudioTrack(nsISupports* aParent,
                     const nsAString& aKind,
                     const nsAString& aLabel,
                     const nsAString& aLanguage)
  : mParent(aParent)
{
  SetDefaultSettings();
  mKind = aKind;
  mLabel = aLabel;
  mLanguage = aLanguage;
  SetIsDOMBinding();
}

AudioTrack::AudioTrack(nsISupports* aParent,
                     AudioTrackList* aAudioTrackList,
                     const nsAString& aKind,
                     const nsAString& aLabel,
                     const nsAString& aLanguage)
  : mParent(aParent)
  , mAudioTrackList(aAudioTrackList)
{
  SetDefaultSettings();
  mKind = aKind;
  mLabel = aLabel;
  mLanguage = aLanguage;
  SetIsDOMBinding();
}

void
AudioTrack::SetDefaultSettings()
{
  // set mKind to empty?
  mEnabled = false;
}

AudioTrack::~AudioTrack()
{
}

JSObject*
AudioTrack::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aScope)
{
  return AudioTrackBinding::Wrap(aCx, aScope, this);
}

void
AudioTrack::SetEnabled(bool aValue)
{
  mEnabled = aValue;
}

AudioTrackList*
AudioTrack::GetAudioTrackList()
{
  return mAudioTrackList;
}

void
AudioTrack::SetAudioTrackList(AudioTrackList* aAudioTrackList)
{
  mAudioTrackList = aAudioTrackList;
}

} // namespace dom
} // namespace mozilla
