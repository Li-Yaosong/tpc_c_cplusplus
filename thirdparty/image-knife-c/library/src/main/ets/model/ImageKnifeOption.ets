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
import taskpool from '@ohos.taskpool';
import common from '@ohos.app.ability.common'
import { CacheStrategy, ImageKnifeData,EventImage } from './ImageKnifeData';
import { drawing } from '@kit.ArkGraphics2D';
import { ImageKnifeRequest } from './ImageKnifeRequest';

export interface HeaderOptions {
  key: string;
  value: Object;
}
interface AnimatorType {
  state?: AnimationStatus
  iterations?: number
  reverse?: boolean
  onStart?:()=>void
  onFinish?:()=>void
  onPause?:()=>void
  onCancel?:()=>void
  onRepeat?:()=>void
}
@ObservedV2
export class AnimatorOption {
  @Trace
  state?: AnimationStatus = AnimationStatus.Running
  @Trace
  iterations?: number = -1
  @Trace
  reverse?: boolean = false
  @Trace
  onStart?:()=>void
  @Trace
  onFinish?:()=>void
  @Trace
  onPause?:()=>void
  @Trace
  onCancel?:()=>void
  @Trace
  onRepeat?:()=>void
  constructor(option?:AnimatorType) {
    this.state = option?.state
    this.iterations = option?.iterations
    this.reverse = option?.reverse
    this.onStart = option?.onStart
    this.onFinish = option?.onFinish
    this.onPause = option?.onPause
    this.onCancel = option?.onCancel
    this.onRepeat = option?.onRepeat
  }
}
interface ImageOption {
  // 主图资源
  loadSrc: string | PixelMap | Resource
  // 占位图
  placeholderSrc?: string | PixelMap | Resource
  // 失败占位图
  errorholderSrc?: string | PixelMap | Resource
  headerOption?: Array<HeaderOptions>;
  // 自定义缓存关键字
  signature?: string

  // 主图填充效果
  objectFit?: ImageFit
  // 占位图填充效果
  placeholderObjectFit?: ImageFit
  // 错误图填充效果
  errorholderObjectFit?: ImageFit
  customGetImage?: (context: Context, src: string | PixelMap | Resource) => Promise<ArrayBuffer | undefined>
  border?: BorderOptions
  // 缓存策略
  writeCacheStrategy?: CacheStrategy
  // 仅使用缓存加载数据
  onlyRetrieveFromCache?: boolean;
  priority?: taskpool.Priority
  context?: common.UIAbilityContext;
  progressListener?: (progress: number) => void;
  onLoadListener?: OnLoadCallBack | undefined;
  onComplete?:(event:EventImage | undefined) => void
  drawingColorFilter?: ColorFilter | drawing.ColorFilter
}

export interface HttpRequestOption {
  caPath?: string  // 自定义证书路径
  connectTimeout?: number // 连接超时
  readTimeout?: number // 读取超时
}

@Observed
export class ImageKnifeOption {
  // 主图资源
  loadSrc: string | PixelMap | Resource = "";
  // 占位图
  placeholderSrc?: string | PixelMap | Resource;
  // 失败占位图
  errorholderSrc?: string | PixelMap | Resource;
  headerOption?: Array<HeaderOptions>;
  // 自定义缓存关键字
  signature?: string;

  // 主图填充效果
  objectFit?: ImageFit
  // 占位图填充效果
  placeholderObjectFit?: ImageFit
  // 错误图填充效果
  errorholderObjectFit?: ImageFit
  customGetImage?: (context: Context, src: string | PixelMap | Resource) => Promise<ArrayBuffer | undefined>
  border?: BorderOptions
  // 缓存策略
  writeCacheStrategy?: CacheStrategy
  // 仅使用缓存加载数据
  onlyRetrieveFromCache?: boolean = false;
  priority?: taskpool.Priority = taskpool.Priority.LOW
  context?: common.UIAbilityContext;
  progressListener?: (progress: number) => void;
  onLoadListener?: OnLoadCallBack | undefined;
  onComplete?:(event:EventImage | undefined) => void
  drawingColorFilter?: ColorFilter | drawing.ColorFilter

  // 自定义证书路径
  httpOption?: HttpRequestOption

  constructor() {
  }
}

/**
 * 请求回调
 */
export interface OnLoadCallBack {
  // 请求开始
  onLoadStart?: (request?: ImageKnifeRequest) => void;

  // 请求成功
  onLoadSuccess?: (data: string | PixelMap | undefined, imageKnifeData: ImageKnifeData, request?: ImageKnifeRequest) => void;

  // 请求结束
  onLoadFailed?: (err: string, request?: ImageKnifeRequest) => void;
  // 请求取消
  onLoadCancel?: (reason: string, request?: ImageKnifeRequest) => void;
}