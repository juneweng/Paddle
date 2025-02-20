/* Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#include "paddle/fluid/framework/eigen.h"
#include "paddle/fluid/framework/op_registry.h"
#include "paddle/fluid/framework/pten_utils.h"
#include "paddle/fluid/operators/eigen/eigen_function.h"

// only can include the headers in paddle/pten/api dirs
#include "paddle/pten/api/include/core.h"
#include "paddle/pten/api/include/math.h"

namespace paddle {
namespace operators {

// See Note [ Why still keep the original kernel implementation? ]
template <typename DeviceContext, typename T>
class SignKernel : public framework::OpKernel<T> {
 public:
  virtual void Compute(const framework::ExecutionContext& context) const {
    auto* x = context.Input<framework::Tensor>("X");
    auto* out = context.Output<framework::Tensor>("Out");
    auto& dev_ctx = context.device_context<DeviceContext>();
    out->mutable_data<T>(x->place());

    auto pt_x = paddle::experimental::MakePtenDenseTensor(*x);
    auto pt_out = paddle::experimental::MakePtenDenseTensor(*out);

    // call new kernel
    pten::Sign<T>(dev_ctx, *pt_x.get(), pt_out.get());
  }
};

}  // namespace operators
}  // namespace paddle
