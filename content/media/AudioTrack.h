/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 et tw=78: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_AudioTrack_h
#define mozilla_dom_AudioTrack_h

#include "mozilla/dom/AudioTrackBinding.h"
#include "nsCycleCollectionParticipant.h"
#include "nsString.h"
#include "nsWrapperCache.h"

namespace mozilla {
namespace dom {

class AudioTrackList;

class AudioTrack MOZ_FINAL : public nsISupports,
                             public nsWrapperCache
{
public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(AudioTrack)

  AudioTrack(nsISupports* aParent);
  AudioTrack(nsISupports* aParent,
            const nsAString& aKind,
            const nsAString& aLabel,
            const nsAString& aLanguage);
  AudioTrack(nsISupports* aParent,
            AudioTrackList* aAudioTrackList,
            const nsAString& aKind,
            const nsAString& aLabel,
            const nsAString& aLanguage);

  void SetDefaultSettings();

  virtual ~AudioTrack();

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

  bool Enabled() const
  {
    return mEnabled;
  }
  void SetEnabled(bool aValue);

  AudioTrackList* GetAudioTrackList();
  void SetAudioTrackList(AudioTrackList* aAudioTrackList);

private:

  nsCOMPtr<nsISupports> mParent;
  nsRefPtr<AudioTrackList> mAudioTrackList;

  nsString mKind;
  nsString mLabel;
  nsString mLanguage;
  nsString mType;
  nsString mId;
  bool mEnabled;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_AudioTrack_h
