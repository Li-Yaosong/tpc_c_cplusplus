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
import { ImageKnifeOption } from './ImageKnifeOption';
import common from '@ohos.app.ability.common';
import { ImageKnifeData } from './ImageKnifeData';


export class ImageKnifeRequest {
  id: string = ""
  requestState: ImageKnifeRequestState = ImageKnifeRequestState.PROGRESS
  componentWidth: number = 0
  componentHeight: number = 0
  drawPlayHolderSuccess: boolean = false
  imageKnifeOption: ImageKnifeOption
  context: common.UIAbilityContext
  componentVersion: number = 0
  headers: Map<string,Object> = new Map<string,Object>()
  imageKnifeData?: ImageKnifeData
  constructor(option: ImageKnifeOption,
              uIAbilityContext: common.UIAbilityContext,
              width: number,
              height: number,
              version: number) {
    this.imageKnifeOption = option
    this.context = uIAbilityContext
    this.componentWidth = width
    this.componentHeight = height
    this.componentVersion = version
  }
  // RequestOption调用header对于的方法
  addHeader(key: string, value: Object) {
    this.headers.set(key, value);
  }

  // 全局调用header对应的方法，包含RequestOption的形式
  addHeaderMap(map: Map<string, Object>) {
    map.forEach((value, key) => {
      if (!this.headers.has(key)) {
        this.addHeader(key, value);
      }
    })
  }
}

export enum ImageKnifeRequestState {
  PROGRESS,
  COMPLETE,
  ERROR,
  DESTROY
}
