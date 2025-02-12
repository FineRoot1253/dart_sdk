// Copyright (c) 2022, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

// SharedOptions=--enable-experiment=class-modifiers

// Error when attempting to implement typedef base class outside of its library.

import 'base_class_typedef_lib.dart';

class BTypeDef implements BaseClassTypeDef {
//    ^
// [cfe] The class 'BaseClass' can't be implemented outside of its library because it's a base class.
//                        ^^^^^^^^^^^^^^^^
// [analyzer] COMPILE_TIME_ERROR.INVALID_USE_OF_TYPE_OUTSIDE_LIBRARY
  @override
  int foo = 1;
}

// Testing another layer of typedefs outside of the library.
typedef BaseClassTypeDef2 = BaseClassTypeDef;

class BTypeDef2 implements BaseClassTypeDef2 {
//    ^
// [cfe] The class 'BaseClass' can't be implemented outside of its library because it's a base class.
//                         ^^^^^^^^^^^^^^^^^
// [analyzer] COMPILE_TIME_ERROR.INVALID_USE_OF_TYPE_OUTSIDE_LIBRARY
  @override
  int foo = 1;
}
