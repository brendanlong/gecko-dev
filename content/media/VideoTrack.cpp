/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 et tw=78: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/VideoTrack.h"
#include "mozilla/dom/VideoTrackBinding.h"
#include "mozilla/dom/VideoTrackList.h"
#include "mozilla/dom/HTMLMediaElement.h"
#include "mozilla/dom/HTMLTrackElement.h"

namespace mozilla {
namespace dom {

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_2(VideoTrack, mParent, mVideoTrackList)
NS_IMPL_CYCLE_COLLECTING_ADDREF(VideoTrack)
NS_IMPL_CYCLE_COLLECTING_RELEASE(VideoTrack)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(VideoTrack)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

VideoTrack::VideoTrack(nsISupports* aParent)
  : mParent(aParent)
{
  SetDefaultSettings();
  SetIsDOMBinding();
}

VideoTrack::VideoTrack(nsISupports* aParent,
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

VideoTrack::VideoTrack(nsISupports* aParent,
                     VideoTrackList* aVideoTrackList,
                     const nsAString& aKind,
                     const nsAString& aLabel,
                     const nsAString& aLanguage)
  : mParent(aParent)
  , mVideoTrackList(aVideoTrackList)
{
  SetDefaultSettings();
  mKind = aKind;
  mLabel = aLabel;
  mLanguage = aLanguage;
  SetIsDOMBinding();
}

void
VideoTrack::SetDefaultSettings()
{
  // set mKind to empty?
  mSelected = false;
}

VideoTrack::~VideoTrack()
{
}

JSObject*
VideoTrack::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aScope)
{
  return VideoTrackBinding::Wrap(aCx, aScope, this);
}

void
VideoTrack::SetSelected(bool aValue)
{
  mSelected = aValue;
}

VideoTrackList*
VideoTrack::GetVideoTrackList()
{
  return mVideoTrackList;
}

void
VideoTrack::SetVideoTrackList(VideoTrackList* aVideoTrackList)
{
  mVideoTrackList = aVideoTrackList;
}

} // namespace dom
} // namespace mozilla
