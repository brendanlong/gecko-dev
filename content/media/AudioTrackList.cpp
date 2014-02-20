/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/AudioTrack.h"
#include "mozilla/dom/AudioTrackList.h"
#include "mozilla/dom/AudioTrackListBinding.h"
#include "mozilla/dom/HTMLMediaElement.h"
#include "mozilla/dom/TrackEvent.h"

namespace mozilla {
namespace dom {

NS_IMPL_CYCLE_COLLECTION_INHERITED_2(AudioTrackList,
                                     nsDOMEventTargetHelper,
                                     mGlobal,
                                     mAudioTracks)

NS_IMPL_ADDREF_INHERITED(AudioTrackList, nsDOMEventTargetHelper)
NS_IMPL_RELEASE_INHERITED(AudioTrackList, nsDOMEventTargetHelper)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION_INHERITED(AudioTrackList)
NS_INTERFACE_MAP_END_INHERITING(nsDOMEventTargetHelper)

AudioTrackList::AudioTrackList(nsISupports* aGlobal)
    : mGlobal(aGlobal)
    , mMediaElement(nullptr)
{
  SetIsDOMBinding();
}

AudioTrackList::AudioTrackList(nsISupports* aGlobal, HTMLMediaElement* aMediaElement)
 : mGlobal(aGlobal)
 , mMediaElement(aMediaElement)
{
  SetIsDOMBinding();
}

JSObject*
AudioTrackList::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aScope)
{
  return AudioTrackListBinding::Wrap(aCx, aScope, this);
}

AudioTrack*
AudioTrackList::IndexedGetter(uint32_t aIndex, bool& aFound)
{
  aFound = aIndex < mAudioTracks.Length();
  return aFound ? mAudioTracks[aIndex] : nullptr;
}

already_AddRefed<AudioTrack>
AudioTrackList::AddAudioTrack(const nsAString& aKind,
                            const nsAString& aLabel,
                            const nsAString& aLanguage)
{
  nsRefPtr<AudioTrack> track = new AudioTrack(mGlobal, this, aKind, aLabel, aLanguage);
  AddAudioTrack(track);
  return track.forget();
}

void
AudioTrackList::AddAudioTrack(AudioTrack* aAudioTrack)
{
  if (mAudioTracks.AppendElement(aAudioTrack)) {
    aAudioTrack->SetAudioTrackList(this);
    CreateAndDispatchTrackEventRunner(aAudioTrack, NS_LITERAL_STRING("addtrack"));
  }
}

AudioTrack*
AudioTrackList::GetTrackById(const nsAString& aId)
{
  nsAutoString id;
  for (uint32_t i = 0; i < Length(); i++) {
    mAudioTracks[i]->GetId(id);
    if (aId.Equals(id)) {
      return mAudioTracks[i];
    }
  }
  return nullptr;
}

void
AudioTrackList::RemoveAudioTrack(AudioTrack* aTrack)
{
  if (mAudioTracks.RemoveElement(aTrack)) {
    CreateAndDispatchTrackEventRunner(aTrack, NS_LITERAL_STRING("removetrack"));
  }
}

class AudioTrackEventRunner MOZ_FINAL: public nsRunnable
{
public:
  AudioTrackEventRunner(AudioTrackList* aList, nsIDOMEvent* aEvent)
    : mList(aList)
    , mEvent(aEvent)
  {}

  NS_IMETHOD Run() MOZ_OVERRIDE
  {
    return mList->DispatchTrackEvent(mEvent);
  }

private:
  nsRefPtr<AudioTrackList> mList;
  nsRefPtr<nsIDOMEvent> mEvent;
};

nsresult
AudioTrackList::DispatchTrackEvent(nsIDOMEvent* aEvent)
{
  return DispatchTrustedEvent(aEvent);
}

void
AudioTrackList::CreateAndDispatchChangeEvent()
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

  nsCOMPtr<nsIRunnable> eventRunner = new AudioTrackEventRunner(this, event);
  NS_DispatchToMainThread(eventRunner, NS_DISPATCH_NORMAL);
}

void
AudioTrackList::CreateAndDispatchTrackEventRunner(AudioTrack* aTrack,
                                                 const nsAString& aEventName)
{
  TrackEventInit eventInit;
  eventInit.mBubbles = false;
  eventInit.mCancelable = false;
  eventInit.mTrack = aTrack->GetWrapper();
  nsRefPtr<TrackEvent> event =
    TrackEvent::Constructor(this, aEventName, eventInit);

  // Dispatch the TrackEvent asynchronously.
  nsCOMPtr<nsIRunnable> eventRunner = new AudioTrackEventRunner(this, event);
  NS_DispatchToMainThread(eventRunner, NS_DISPATCH_NORMAL);
}

HTMLMediaElement*
AudioTrackList::GetMediaElement()
{
  return mMediaElement;
}

void
AudioTrackList::SetMediaElement(HTMLMediaElement* aMediaElement)
{
  mMediaElement = aMediaElement;
}

} // namespace dom
} // namespace mozilla
