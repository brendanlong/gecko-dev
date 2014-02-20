/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 et tw=78: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_VideoTrack_h
#define mozilla_dom_VideoTrack_h

#include "mozilla/dom/VideoTrackBinding.h"
#include "nsCycleCollectionParticipant.h"
#include "nsString.h"
#include "nsWrapperCache.h"

namespace mozilla {
namespace dom {

class VideoTrackList;

class VideoTrack MOZ_FINAL : public nsISupports,
                             public nsWrapperCache
{
public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(VideoTrack)

  VideoTrack(nsISupports* aParent);
  VideoTrack(nsISupports* aParent,
            const nsAString& aKind,
            const nsAString& aLabel,
            const nsAString& aLanguage);
  VideoTrack(nsISupports* aParent,
            VideoTrackList* aVideoTrackList,
            const nsAString& aKind,
            const nsAString& aLabel,
            const nsAString& aLanguage);

  void SetDefaultSettings();

  virtual ~VideoTrack();

  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aScope) MOZ_OVERRIDE;

  nsISupports* GetParentObject() const
  {
    return mParent;
  }

  void GetKind(nsAString& aKind) const
  {
    aKind = mKind;
  }
  void GetLabel(nsAString& aLabel) const
  {
    aLabel = mLabel;
  }
  void GetLanguage(nsAString& aLanguage) const
  {
    aLanguage = mLanguage;
  }
  void GetId(nsAString& aId) const
  {
    aId = mId;
  }

  bool Selected() const
  {
    return mSelected;
  }
  void SetSelected(bool aValue);

  VideoTrackList* GetVideoTrackList();
  void SetVideoTrackList(VideoTrackList* aVideoTrackList);

private:

  nsCOMPtr<nsISupports> mParent;
  nsRefPtr<VideoTrackList> mVideoTrackList;

  nsString mKind;
  nsString mLabel;
  nsString mLanguage;
  nsString mType;
  nsString mId;
  bool mSelected;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_VideoTrack_h
