/* Copyright (C) 2023 Jonas Strindlund - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the YYZ license, which unfortunately won't be
 * written for another century (2112).
 *
 * You should have received a copy of the YYZ license with
 * this file. If not, please write to: jonas.strindlund@gmail.com
 * or visit : ByTor.yyz
 *
 */

/*
1 Error detection

The first step in handling errors and exceptions is to detect them as early and accurately as possible.
You can use various techniques to check for errors and exceptions, such as assertions, error codes,
return values, error flags, status registers, watchdog timers, and hardware interrupts.
Depending on the type and severity of the error or exception, you can choose to ignore, log, report, recover, or halt the system.

2 Error handling

Once you detect an error or exception, you need to decide how to handle it.
You can use different methods to handle errors and exceptions, such as try-catch blocks,
error handlers, exception handlers, fault handlers, and error recovery routines.
The choice of method depends on the nature and impact of the error or exception, as well as the design and requirements of your system.

3 Error reporting

Another important aspect of handling errors and exceptions is to report them to the user,
the developer, or the external system. You can use different ways to report errors and exceptions,
such as LED indicators, LCD displays, serial ports, network sockets, debuggers, loggers, and trace tools.
The level of detail and frequency of reporting depends on the purpose and audience of the report.

4 Error prevention

The best way to handle errors and exceptions is to prevent them from happening in the first place.
You can use various techniques to prevent errors and exceptions, such as code reviews, static analysis,
unit testing, integration testing, system testing, code coverage, memory management, and coding standards.
By following these techniques, you can improve the quality and robustness of your embedded software.

5 Error recovery

Sometimes, errors and exceptions are unavoidable or unpredictable, and you need to recover from them gracefully.
You can use different strategies to recover from errors and exceptions, such as resetting, rebooting, restarting,
re-initializing, retrying, fallback, and fail-safe. The choice of strategy depends on the availability and feasibility
of recovery options, as well as the expected behavior and performance of your system.

6 Error mitigation

Finally, you need to consider how to mitigate the consequences and risks of errors and exceptions.
You can use different measures to mitigate errors and exceptions, such as redundancy, diversity,
isolation, fault tolerance, fail-over, and fault injection. By applying these measures,
you can increase the resilience and reliability of your embedded software.

The best approach is to return errors and check for them. Use the warn_unused_result attribute and -Werror
NOTE: THis method is not feasable when using tons of libraries not checking return values / use static code analyzer on YOUR code instead

You can break errors mentally into two groups: 1) A hard error, like out of memory or similar.
A true unexpected. 2) A soft error, like maybe sqrt() isn't designed to take negative numbers,
or it needs to return NaN in some sort of way. Most errors are the later kind.

By breaking errors into two mental groups, it helps categorize the kind of error you're dealing with,
allowing for idioms and best practices to form within your group and code base.

Using magic numbers is a kind of code smell, though sometimes is necessary to continue a style from older code.
Therefor, when reasonable, you shouldn't return a negative number representing an error code.

    possible errors (file not found, invalid user input, ...) shall be handled by returned error code

    improbable errors (no more system memory) shall be handle by abortion or exception,

    logic errors shall be handled by assertion (abuse them, they're goods)


Always enable HW watchdog that will reset the system in case the MCU ends up in a weird state in some way that was not anticipated at all.
Even in case of watchdog resets, the reason for the reboot should be easily identifiable in the log output.
Use asserts for user/programmer errors.
For errors that you can't entirely control, but could be a result of poor software use a combination of the asserts and error return codes.


When dealing with things like embedded file systems or streams, it may be helpful to have each "file"
or stream maintain a latching error state, and have three kinds of function return values: success,
transient failure, and latched failure, with "transient failure" only being an option for call that
indicate that they are expected. Using this approach will make it possible to write code that performs
a sequence of read or write requests and then check whether the sequence as a whole succeeded without
having to check the results of individual operations.

Note that it even if calls which could return transient failures would not latch failure conditions
themselves, the return values should still distinguish latching and transient failures to ensure that
loops that would retry operations in case of transient failures don't get stuck repeating endlessly
because of an earlier latching failure.

In general I return 0 for success or POSIX errno.h numbers on error, as is standard for Unix and Linux systems work.
I can’t see any good reason to do things differently for bare metal work.

   This turned out to be a small essay. Will revisit
 */
#include <stdio.h>
#include <errno.h>

#include "pico/stdlib.h"
#include "errorhandler.h"

/** \brief Macro if you want to "puts(FILE_LINE)"
 *
 */
#define STR2(X) #X
#define STR(X) STR2(X)
#define FILE_LINE __FILE_NAME__ ":" STR(__LINE__) " "
extern void restart();

/*
Assertions shouldn't "crash" your system when they fail.
Assertions are a fail-safe that resets the system to a known-good state so that your code can run properly.
You should use assertions anytime you check for a condition that should not fail in practice.
"Expect the unexpected"

For this reason, assertions should be left enabled in release code,
so you can find out if and why fielded devices are encountering problems that should never occur in practice.
If your code fails assertions frequently, it's going to annoy users, but it's better that the unit is actually running the way it's supposed to be.
And yes, if your code fails assertions frequently, you damn well better fix your code, and you can only do that if you know its happening and why.

Because it simplifies error handling for the caller. If this an error that should never happen
-- which would typically be the case when opening a HW driver on an embedded device --
if you guarantee that assertions are always enabled, the caller should just do this:

assert(NO_ERR == driver_open(&settings));

*/

/** \brief __assert_fail will log the assertion and return the system to "reset" state
 *
 *         Assertions shouldn't "crash" your system when they fail.
 *         Assertions are a fail-safe that resets the system to a known-good state so that your code can run properly.
 *         You should use assertions anytime you check for a condition that should not fail in practice.
 *         "Expect the unexpected"
 *
 * \ref assert
 * \param assertion const char*
 * \param filename const char*
 * \param linenumber unsigned int
 * \param function const char*
 * \return void
 *
 */
void __assert_fail(const char* assertion, const char* filename, unsigned int linenumber, const char* function)
{
// TODO (jss#1#03/23/25): Redirect this to a persistent log once logging is in place.
   fprintf(stderr, "%s %d\n", filename, linenumber);
   fprintf(stderr, "%s\n", assertion);
   fprintf(stderr, "%s\n", function);

   restart();
}
