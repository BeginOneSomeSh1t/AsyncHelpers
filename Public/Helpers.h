#pragma once
#include "CoreMinimal.h"

namespace AsyncHelpers
{
	inline TFuture<void> ExecuteOnGameThread(TFunction<void()> Function)
	{
		TPromise<void> Promise;
		auto Future = Promise.GetFuture();
		
		if (IsInGameThread())
		{
			Promise.SetValue();
			Function();
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [CapturedFunctor = MoveTemp(Function), CapturedPromise = MoveTemp(Promise)]() mutable
			{
				CapturedFunctor();
				CapturedPromise.SetValue();
			});
		}

		return Future;
	}
}