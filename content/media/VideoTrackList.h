/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 et tw=78: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_VideoTrackList_h
#define mozilla_dom_VideoTrackList_h

#include "mozilla/dom/VideoTrack.h"
#include "nsCycleCollectionParticipant.h"
#include "nsDOMEventTargetHelper.h"

namespace mozilla {
namespace dom {

class HTMLMediaElement;
class TrackEvent;
class TrackEventRunner;

class VideoTrackList MOZ_FINAL : public nsDOMEventTargetHelper
{
public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(VideoTrackList, nsDOMEventTargetHelper)

  VideoTrackList(nsISupports* aGlobal);
  VideoTrackList(nsISupports* aGlobal, HTMLMediaElement* aMediaElement);

  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aScope) MOZ_OVERRIDE;

  nsISupports* GetParentObject() const
  {
    return mGlobal;
  }

  uint32_t Length() const
  {
    return mVideoTracks.Length();
  }

  VideoTrack* IndexedGetter(uint32_t aIndex, bool& aFound);

  already_AddRefed<VideoTrack> AddVideoTrack(const nsAString& aKind,
                                           const nsAString& aLabel,
                                           const nsAString& aLanguage);
  VideoTrack* GetTrackById(const nsAString& aId);
  long SelectedIndex();

  void AddVideoTrack(VideoTrack* aVideoTrack);

  void RemoveVideoTrack(VideoTrack* aTrack);
  void DidSeek();

  HTMLMediaElement* GetMediaElement();
  void SetMediaElement(HTMLMediaElement* aMediaElement);

  nsresult DispatchTrackEvent(nsIDOMEvent* aEvent);
  void CreateAndDispatchChangeEvent();

  IMPL_EVENT_HANDLER(change)
  IMPL_EVENT_HANDLER(addtrack)
  IMPL_EVENT_HANDLER(removetrack)

private:
  nsCOMPtr<nsISupports> mGlobal;
  nsTArray< nsRefPtr<VideoTrack> > mVideoTracks;
  HTMLMediaElement* mMediaElement;

  void CreateAndDispatchTrackEventRunner(VideoTrack* aTrack,
                                         const nsAString& aEventName);
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_VideoTrackList_h
