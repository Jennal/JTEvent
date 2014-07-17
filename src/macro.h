/************************************************************
The MIT License (MIT)

Copyright (c) 2014 Jennal (jennalcn@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************/

#ifndef __JT_MACRO_H__
#define __JT_MACRO_H__

#include <cassert>

// language sugar
#ifndef __abstract
#define __abstract
#endif /* abstract */
#ifndef __interface
#define __interface  __abstract class
#endif /* interface */
#ifndef __final
#define __final /* final means you can't inherit from it */
#endif /* __finnal */

#ifndef JTASSERT
#define JTASSERT(cond, msg) \
	if ( ! cond) printf("%s(%d)-%s: %s\n",__FILE__,__LINE__,__FUNCTION__, msg); \
	assert(cond);
#endif

#ifndef __weak_reference
#define __weak_reference /* weak reference means you don't need to release its memory */
#endif /* __weak_reference */
#ifndef __weak_item_reference
#define __weak_item_reference /* weak item reference means you don't need to release its items memory */
#endif /* __weak_item_reference */

#ifndef USING_NS_STD
#define USING_NS_STD using namespace std
#endif /* USING_NS_STD */

#ifdef __cplusplus
#define NS_JT_BEGIN   namespace JT {
#define NS_JT_END     }
#define USING_NS_JT   using namespace JT
#else
#define NS_JT_BEGIN 
#define NS_JT_END 
#define USING_NS_JT
#endif /* End JT */

#ifndef __weak_reference
#define __weak_reference /* weak reference means you don't need to release its memory */
#endif /* __weak_reference */

#endif /* __JT_MACRO_H__ */