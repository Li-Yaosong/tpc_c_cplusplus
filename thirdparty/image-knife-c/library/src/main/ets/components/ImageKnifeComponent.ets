/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the 'License');
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an 'AS IS' BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import { ImageKnifeOption } from '../model/ImageKnifeOption';
import common from '@ohos.app.ability.common';
import { NodeContent } from '@ohos.arkui.node';
import nativeNode from 'libimageknifec.so';

@Component
export struct ImageKnifeComponent {
  // private componentId:string = util.generateRandomUUID(true)
  private  componentId:string = ""
  syncLoad: boolean = false
  private currentContext: common.UIAbilityContext | undefined = undefined
  @Watch('watchImageKnifeOption') @ObjectLink imageKnifeOption: ImageKnifeOption
  private rootSlot = new NodeContent();
  private componentVersion: number = 0

  aboutToAppear(): void {
    this.componentId = this.getUniqueId().toString()
    this.fillImageKnifeOptionContext()
    nativeNode.createNativeRoot(this.rootSlot, this.componentId, this.imageKnifeOption, this.syncLoad)
  }

  aboutToDisappear(): void {
    nativeNode.destroyNativeRoot(this.componentId)
  }

  aboutToRecycle() {
    nativeNode.clearNativeRoot(this.componentId)
  }

  watchImageKnifeOption() {
    this.componentVersion++
    this.fillImageKnifeOptionContext()
    nativeNode.updateNativeRoot(this.componentId, this.imageKnifeOption, this.componentVersion, this.syncLoad)
  }

  fillImageKnifeOptionContext() {
    if (this.imageKnifeOption.context === undefined) {
      this.imageKnifeOption.context = this.getCurrentContext()
    }
  }

  build() {
    ContentSlot(this.rootSlot)
  }

  getCurrentContext(): common.UIAbilityContext {
    if (this.currentContext == undefined) {
      this.currentContext = getContext(this) as common.UIAbilityContext
    }
    return this.currentContext
  }
}