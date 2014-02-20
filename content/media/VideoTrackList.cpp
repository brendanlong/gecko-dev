/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/HTMLMediaElement.h"
#include "mozilla/dom/TrackEvent.h"
#include "mozilla/dom/VideoTrack.h"
#include "mozilla/dom/VideoTrackList.h"
#include "mozilla/dom/VideoTrackListBinding.h"

namespace mozilla {
namespace dom {

NS_IMPL_CYCLE_COLLECTION_INHERITED_2(VideoTrackList,
                                     nsDOMEventTargetHelper,
                                     mGlobal,
                                     mVideoTracks)

NS_IMPL_ADDREF_INHERITED(VideoTrackList, nsDOMEventTargetHelper)
NS_IMPL_RELEASE_INHERITED(VideoTrackList, nsDOMEventTargetHelper)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION_INHERITED(VideoTrackList)
NS_INTERFACE_MAP_END_INHERITING(nsDOMEventTargetHelper)

VideoTrackList::VideoTrackList(nsISupports* aGlobal)
    : mGlobal(aGlobal)
    , mMediaElement(nullptr)
{
  SetIsDOMBinding();
}

VideoTrackList::VideoTrackList(nsISupports* aGlobal, HTMLMediaElement* aMediaElement)
 : mGlobal(aGlobal)
 , mMediaElement(aMediaElement)
{
  SetIsDOMBinding();
}

JSObject*
VideoTrackList::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aScope)
{
  return VideoTrackListBinding::Wrap(aCx, aScope, this);
}

VideoTrack*
VideoTrackList::IndexedGetter(uint32_t aIndex, bool& aFound)
{
  aFound = aIndex < mVideoTracks.Length();
  return aFound ? mVideoTracks[aIndex] : nullptr;
}

already_AddRefed<VideoTrack>
VideoTrackList::AddVideoTrack(const nsAString& aKind,
                            const nsAString& aLabel,
                            const nsAString& aLanguage)
{
  nsRefPtr<VideoTrack> track = new VideoTrack(mGlobal, this, aKind, aLabel, aLanguage);
  AddVideoTrack(track);
  return track.forget();
}

void
VideoTrackList::AddVideoTrack(VideoTrack* aVideoTrack)
{
  if (mVideoTracks.AppendElement(aVideoTrack)) {
    aVideoTrack->SetVideoTrackList(this);
    CreateAndDispatchTrackEventRunner(aVideoTrack, NS_LITERAL_STRING("addtrack"));
  }
}

VideoTrack*
VideoTrackList::GetTrackById(const nsAString& aId)
{
  nsAutoString id;
  for (uint32_t i = 0; i < Length(); i++) {
    mVideoTracks[i]->GetId(id);
    if (aId.Equals(id)) {
      return mVideoTracks[i];
    }
  }
  return nullptr;
}


long
VideoTrackList::SelectedIndex()
{
  for (uint32_t i = 0; i < Length(); i++) {
    if (mVideoTracks[i]->Selected()) {
      return i;
    }
  }
  return -1;
}

void
VideoTrackList::RemoveVideoTrack(VideoTrack* aTrack)
{
  if (mVideoTracks.RemoveElement(aTrack)) {
    CreateAndDispatchTrackEventRunner(aTrack, NS_LITERAL_STRING("removetrack"));
  }
}

class VideoTrackEventRunner MOZ_FINAL: public nsRunnable
{
public:
  VideoTrackEventRunner(VideoTrackList* aList, nsIDOMEvent* aEvent)
    : mList(aList)
    , mEvent(aEvent)
  {}

  NS_IMETHOD Run() MOZ_OVERRIDE
  {
    return mList->DispatchTrackEvent(mEvent);
  }

private:
  nsRefPtr<VideoTrackList> mList;
  nsRefPtr<nsIDOMEvent> mEvent;
};

nsresult
VideoTrackList::DispatchTrackEvent(nsIDOMEvent* aEvent)
{
  return DispatchTrustedEvent(aEvent);
}

void
VideoTrackList::CreateAndDispatchChangeEvent()
{
  nsCOMPtr<nsIDOMEvent> event;
  nsresult rv = NS_NewDOMEvent(getter_AddRefs(event), this, nullptr, nullptr);
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed to create the error event!");
    return;
  }

  rv = event->InitEvent(NS_LITERAL_STRING("change"), false, false);
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed to init the change event!");
    return;
  }

  event->SetTrusted(true);

  nsCOMPtr<nsIRunnable> eventRunner = new VideoTrackEventRunner(this, event);
  NS_DispatchToMainThread(eventRunner, NS_DISPATCH_NORMAL);
}

void
VideoTrackList::CreateAndDispatchTrackEventRunner(VideoTrack* aTrack,
                                                 const nsAString& aEventName)
{
  TrackEventInit eventInit;
  eventInit.mBubbles = false;
  eventInit.mCancelable = false;
  eventInit.mTrack = aTrack->GetWrapper();
  nsRefPtr<TrackEvent> event =
    TrackEvent::Constructor(this, aEventName, eventInit);

  // Dispatch the TrackEvent asynchronously.
  nsCOMPtr<nsIRunnable> eventRunner = new VideoTrackEventRunner(this, event);
  NS_DispatchToMainThread(eventRunner, NS_DISPATCH_NORMAL);
}

HTMLMediaElement*
VideoTrackList::GetMediaElement()
{
  return mMediaElement;
}

void
VideoTrackList::SetMediaElement(HTMLMediaElement* aMediaElement)
{
  mMediaElement = aMediaElement;
}

} // namespace dom
} // namespace mozilla
