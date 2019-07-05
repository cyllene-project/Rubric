// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)


#include "Rubric.h"
#include "Application.h"

#include <rxcpp/rx.hpp>

using namespace rxcpp;
using namespace rxcpp::rxo;
using namespace rxcpp::rxs;

using namespace rubric;

struct Model {
    struct shared {
        int total = 0;
    };
    std::shared_ptr<shared> data = std::make_shared<shared>();
};


struct ViewModel {

    ViewModel() {}

    explicit ViewModel(Model &m) : m(m) {
        auto &model = *m.data;
    }

    Model m;
};


inline std::function<rxcpp::observable<ViewModel>(rxcpp::observable<ViewModel>)> reportandrepeat() {
    return [](observable<ViewModel> s) {
        return s |
               on_error_resume_next([](std::exception_ptr ep) {
                   std::cerr << rxu::what(ep) << std::endl;
                   return observable<>::empty<ViewModel>();
               }) |
               repeat();
    };
}

using Reducer = std::function<Model(Model &)>;

int main(int argc, char *argv[]) {

    subjects::subject<int> framebus;
    auto frameout = framebus.get_subscriber();
    auto sendframe = [=]() {
        frameout.on_next(1);
    };
    auto frames = framebus.get_observable();

    schedulers::run_loop rl;
    auto mainthreadid = std::this_thread::get_id();
    /* Create coordination for observing on UI thread */
    auto mainthread = observe_on_run_loop(rl);
    /*
     * "The event_loop scheduler is a simple round-robin fixed-size thread pool."
     * http://rxcpp.codeplex.com/discussions/635113
     */
    auto poolthread = observe_on_event_loop();

    composite_subscription lifetime;

    // ==== Model

    /* Reducer is a function that takes Model and returns new Model. The operation of taking a sequence of something
       and aggregating that sequence into single instance of something, is traditionally called reduce.
       In STL traditionally term "accumulate" is used; other languages use term "fold" for similar operation.
       Each reducer here represents a mini-pipeline that listens for incomming data of interest and calculates
       result from them and/or produce side effect.

       Definition of reduce in Rx:
         http://reactivex.io/documentation/operators/reduce.html
    */
    std::vector<observable<Reducer>> reducers;

    // do stuff

    // combine things that modify the model
    auto actions = iterate(reducers)
        // give the reducers to the UX
        .merge(mainthread);
    //
    // apply reducers to the model (Flux architecture)
    //
    auto models = actions
        .scan(Model{}, [=](Model &m, Reducer &f) {
            try {
                auto r = f(m);
                return r;
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
                return std::move(m);
            }
        })
        // only view model updates every 200ms
        .sample_with_time(std::chrono::milliseconds(200), mainthread)
        .publish()
        .ref_count()
        .as_dynamic();

    // ==== View

    auto viewModels = models
        // if the processing of the model takes too long, skip until caught up
        .filter([=](const Model &m) {
            return false;
        })
        .start_with(Model{})
        .map([](Model &m) {
            return ViewModel{m};
        })
        .as_dynamic();

    auto draw = frames
        .with_latest_from(rxu::take_at<1>(), viewModels)
        .tap([=](const ViewModel &) {
            auto renderthreadid = std::this_thread::get_id();
            if (mainthreadid != renderthreadid) {
                std::cerr << "render on wrong thread!" << std::endl;
                std::terminate();
            }
        })
        .replay(1)
        .ref_count()
        .as_dynamic();

    std::vector<observable<ViewModel>> renderers;

    renderers.push_back(
            draw.tap([=](const ViewModel &vm) { })
            | reportandrepeat());

    // subscribe to everything!
    iterate(renderers)
            .merge()
            | subscribe<ViewModel>(lifetime, [](const ViewModel &) {});


    while (lifetime.is_subscribed()) {

        if (!lifetime.is_subscribed()) {
            break;
        }

        while (!rl.empty() && rl.peek().when < rl.now()) {
            rl.dispatch();
        }

        sendframe();

        while (!rl.empty() && rl.peek().when < rl.now()) {
            rl.dispatch();
        }
        // Rendering
    }

    return 0;

}