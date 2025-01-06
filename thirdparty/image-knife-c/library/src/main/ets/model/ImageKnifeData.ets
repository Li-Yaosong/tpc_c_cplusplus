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
import { HeaderOptions } from './ImageKnifeOption'
import { ImageKnifeRequest } from './ImageKnifeRequest'
import common from '@ohos.app.ability.common';
import { Size } from '@kit.ArkUI'

export interface ImageKnifeData {
  source: PixelMap | string, // url
  imageWidth: number, // 原始宽高大小
  imageHeight: number,
  bufSize?: number, // 图片的字节数
  type?:string,
  imageAnimator?: Array<ImageFrameInfo>
  frameCount ?: number // 帧
  decodeImages?: Array<DecodeImageInfo> //Image组件或者ImageAnimator组件可以加载一张或者多张
  timeInfo?: TimeInfo // 加载图片的各个时间点
  errorInfo?: ErrorInfo // 错误
}

/**
 * 解码后的图片的size
 */
export interface DecodeImageInfo {
  contentWidth ?: number // 解码后宽高
  contentHeight?: number
  contentSize ?: number // 大小
}

/**
 * 加载的错误信息
 */
export interface ErrorInfo {
  phase: string, //图片加载阶段信息，如：网络加载阶段，缓存获取阶段及其解码阶段等
  code: number,
  httpCode?: number
}

/**
 * load检查时间点
 */
export interface TimeInfo {
  requestStartTime?: number,
  requestEndTime?: number,
  requestCancelTime?: number,
  memoryCheckStartTime?: number,
  memoryCheckEndTime?: number,
  diskCheckStartTime?: number,
  diskCheckEndTime?: number,
  netRequestStartTime?: number,
  netRequestEndTime?: number,
  decodeStartTime?: number,
  decodeEndTime?: number,
}


/**
 * onComplete成功回调
 */
export interface EventImage {
  width: number;
  height: number;
  componentWidth: number;
  componentHeight: number;
  loadingStatus: number;
  contentWidth: number;
  contentHeight: number;
  contentOffsetX: number;
  contentOffsetY: number;
}
/**
 * 缓存策略
 */
export enum CacheStrategy {
  // 默认-写入/读取内存和文件缓存
  Default = 0,
  // 只写入/读取内存缓存
  Memory = 1,
  // 只写入/读取文件缓存
  File = 2
}

/**
 * 区分是src,placehodler,还是error_holder
 */
export enum ImageKnifeRequestSource {
  SRC,
  PLACE_HOLDER,
  ERROR_HOLDER
}


export interface ImageKnifeRequestWithSource {
  request: ImageKnifeRequest
  source: ImageKnifeRequestSource
}

/**
 * request子线程处理时的返回
 */
export interface RequestJobResult {
  pixelMap: PixelMap | string | undefined
  bufferSize: number
  fileKey: string
  loadFail?: string,
  size?:Size,
  type?: string,
  pixelMapList?:Array<PixelMap>,
  delayList?: Array<number>
}


