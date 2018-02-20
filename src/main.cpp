#include <arrow/api.h>
#include "arrow/builder.h"
#include "arrow/memory_pool.h"
#include "arrow/status.h"

#include "common/common.h"

#include "datum/data_vector.h"
#include "datum/buffer_pool.h"

#include <folly/futures/Future.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <future>

#include "operators/micro/select.h"
#include "csv_helper.h"

#include <sstream>
#include <string>
#include <structures/lock_free_stack.h>
#include <structures/lock_free_stack_hp.h>
#include <util/catalog.h>
#include <plan/plan_helper.h>
//#include <structures/stack_sp.h>

//int askForBuf(std::shared_ptr<BufferPool<int,ArrayVector<int>>> bp);
//int askForBuf(std::shared_ptr<BufferPool<int,ArrayVector<int>>> bp)  {
//
//    std::stringstream ss;
//    std::thread::id this_id = std::this_thread::get_id();
//
//    ss << "thread " << this_id << " activated...\n";
//    std::cout << ss.str();
//    auto it = bp->getBuffer();
//
//    BufferState st = (*it)->getState();
//    if(st == BufferState::PRODUCED) {
//        std::cout << "Asked for buffer and got it: " << std::endl;
//    }
//    //using namespace std::chrono_literals;
//    //std::this_thread::sleep_for(1s);
//    int res = -1;
//    for(size_t i = 0; i < 1'000'000'000; i++)   {
//        res = (i * i) % 1019;
//    }
//
//    bp->freeBuffer(it);
//    std::cout << "Freed buffer as well" << std::endl;
//    return res;
//}

int askForBuf(std::shared_ptr<BufferPool<ArrayVector<int>>> bp);
int askForBuf(std::shared_ptr<BufferPool<ArrayVector<int>>> bp)  {

    std::stringstream ss;
    std::thread::id this_id = std::this_thread::get_id();

    ss << "thread " << this_id << " activated...\n";
    std::cout << ss.str();
    auto buff = bp->getBuffer();

    BufferState st = buff->getState();
    if(st == BufferState::PRODUCED) {
        std::cout << "Asked for buffer and got it: " << std::endl;
    }
    //using namespace std::chrono_literals;
    //std::this_thread::sleep_for(1s);
    int res = -1;
    for(size_t i = 0; i < 1'000'000'000; i++)   {
        res = (i * i) % 1019;
    }

    bp->freeBuffer(buff);
    std::cout << "Freed buffer as well" << std::endl;
    return res;
}

int askForBuf_();
int askForBuf_()  {

    std::stringstream ss;
    std::thread::id this_id = std::this_thread::get_id();

    ss << "thread " << this_id << " activated...\n";
    std::cout << ss.str();
    auto buff = ArrayVector<int>::getBuffer();

    BufferState st = buff->getState();
    if(st == BufferState::PRODUCED) {
        std::cout << "Asked for buffer and got it: " << std::endl;
    }
    //using namespace std::chrono_literals;
    //std::this_thread::sleep_for(1s);
    int res = -1;
    for(size_t i = 0; i < 1'000'000'000; i++)   {
        res = (i * i) % 1019;
    }

    ArrayVector<int>::freeBuffer(buff);
    std::cout << "Freed buffer as well" << std::endl;
    return res;
}

int compute(int id);
int compute(int id)  {

    std::stringstream ss;
    std::thread::id this_id = std::this_thread::get_id();

    ss << "worker " << id << ": thread " << this_id << " activated...\n";
    std::cout << ss.str();

    int res = -1;
    for(size_t i = 0; i < 1'000'000'000; i++)   {
        res = (i * i) % 1019;
    }
    std::cout << "worker " << id << ": thread " << this_id << " quitting...\n";
    return res;
}


void push(lock_free_stack<int>* s);
void push(lock_free_stack<int>* s)
{
    std::stringstream ss;
    for (int i = 0; i < 10; ++i) {
        ss << "pushing " << i << std::endl;
        std::cout << ss.str();
        ss.str("");
        s->push(i);
    }
}

void pop(lock_free_stack<int>* s);
void pop(lock_free_stack<int>* s)
{
    std::stringstream ss;
    int count = 0;
    std::shared_ptr<int> e;
    while (count < 10) {
        e = s->pop();
        if (e) {
            ss << "popping " << *e << std::endl;
            std::cout << ss.str();
            ss.str("");
            ++count;
        }
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;


//    {
//        std::cout << "Sel Test - init v1" << std::endl;
//        //input
//        auto testData = std::make_unique<int[]>(VECTORSIZE);
//        for(int i = 0; i < VECTORSIZE; i++) {
//            testData[i] = i + 1;
//        }
//        auto data(std::make_unique<std::array<int,10>>());
//        std::copy(testData.get(),testData.get() + VECTORSIZE,(*data).begin());
//        std::bitset<VECTORSIZE> selVector;
//
//        select_gt_branching(data,7,10,selVector);
//        std::cout << selVector.count() << std::endl;
//    }
//
//    {
//        std::cout << "Sel Test - init v2" << std::endl;
//        //input
//        int testData[] = {1,2,3,4,5,6,7,8,9,10};
//        auto data(std::make_unique<std::array<int,10>>());
//        //*data = {{1,2,3,4,5,6,7,8,9,10}};
//        std::copy(std::begin(testData),std::end(testData),(*data).begin());
//        std::bitset<VECTORSIZE> selVector;
//
//        select_gt_branching(data,7,10,selVector);
//        std::cout << selVector.count() << std::endl;
//    }
//
//    {
//        std::cout << "Arrow Test" << std::endl;
//        //Build Arrow Structure from
//        using arrow::DoubleBuilder;
//        using arrow::Int64Builder;
//        using arrow::ListBuilder;
//
//        arrow::MemoryPool *pool = arrow::default_memory_pool();
//        Int64Builder col1_builder(pool);
//        DoubleBuilder col2_builder(pool);
//
////        arrow::ListBuilder list_builder(
////                arrow::default_memory_pool(),
////                std::make_unique<arrow::DoubleBuilder> (arrow::default_memory_pool()),
////                arrow::list(arrow::float64()));
//
//        std::shared_ptr<DoubleBuilder> list_content_builder =
//            std::make_shared<arrow::DoubleBuilder> (pool);
//
//
//        ListBuilder list_builder(pool, list_content_builder);
////        DoubleBuilder *list_content_builder =
////                        dynamic_cast<arrow::DoubleBuilder*>(list_builder.value_builder().get());
//
//        //ListBuilder(MemoryPool* pool, std::shared_ptr<ArrayBuilder> value_builder,
//        //const std::shared_ptr<DataType>& type = nullptr);
//
//
////        std::unique_ptr<DoubleBuilder> list_content_builder =
////            std::make_unique<arrow::DoubleBuilder> (pool);
////
////        //After a call to move, we cannot make any assumptions about the value of the moved-from subject
////        //This constructor gets a copy of components_values_builder that's created via move construction
////        ListBuilder list_builder(pool, std::move(list_content_builder));
//////        arrow::ListBuilder val2_builder(
//////                arrow::default_memory_pool(),
//////                std::make_unique<arrow::DoubleBuilder> (arrow::default_memory_pool()),
//////                arrow::list(arrow::float64()));
//
//
//        //Loop over our existing data and insert it into the builders.
//        //The Append calls here may fail (e.g. we cannot allocate enough additional memory).
//        //Thus we need to check their return values.
//        // For more information on these values, check the documentation about arrow::Status.
//        for(int i = 0; i < VECTORSIZE; i++)  {
//            //ARROW_RETURN_NOT_OK(col1_builder.Append(i));
//            auto st = col1_builder.Append(i + 6);
//            if(!st.ok())    {
//                std::cout << "Int col population " << i << " failed" << std::endl;
//                return -1;
//            }
//            st = col2_builder.Append(i + 1.0);
//            if(!st.ok())    {
//                std::cout << "Double col population " << i << " failed" << std::endl;
//                return -1;
//            }
//
//
//            // Indicate the start of a new list row.
//            // This will memorise the current offset in the values builder.
//            list_builder.Append();
//
//            std::vector<double> tmpVec = {1.0, 2.0, 3.0, 4.0};
//            // Store the actual values. The final nullptr argument tells the underlying
//            // builder that all added values are valid, i.e. non-null.
//            st = list_content_builder->Append(tmpVec.data(), tmpVec.size(), nullptr);
//            for(auto& elem : tmpVec)  {
//                std::cout << "le elem " << elem << std::endl;
//                st = list_content_builder->Append(elem);
//            }
//
//
//            if(!st.ok())    {
//                std::cout << "List population " << i << " failed" << std::endl;
//                return -1;
//            }
//        }
//
//        //finalise the arrays, declare the (type) schema and combine them into a single arrow::Table
//        std::shared_ptr<arrow::Array> int_array;
//        auto st = col1_builder.Finish(&int_array);
//        std::shared_ptr<arrow::Array> double_array;
//        st = col2_builder.Finish(&double_array);
//        std::shared_ptr<arrow::Array> list_array;
//        st = list_builder.Finish(&list_array);
//        std::vector<std::shared_ptr<arrow::Field>> schema_vector = {
//                arrow::field("intEx", arrow::int64()),
//                arrow::field("doubleEx", arrow::float64())
////                ,
////                arrow::field("listEx", arrow::list(arrow::float64()))
//        };
//        auto schema = std::make_shared<arrow::Schema>(schema_vector);
//        std::shared_ptr<arrow::Table> table;
//        st = MakeTable(schema, {int_array, double_array /*, list_array*/}, &table);
//
//        //read data back
//        int chunks = table->column(0)->data()->num_chunks();
//        std::cout << chunks << " chunks to be read" << std::endl;
//        //Testing col0 only
//        for(int i = 0; i < chunks; i++) {
//            auto col1 = std::static_pointer_cast<arrow::Int64Array>(table->column(0)->data()->chunk(i));
//            auto col2 = std::static_pointer_cast<arrow::DoubleArray>(table->column(1)->data()->chunk(i));
//
//            std::cout << "Chunk Length: " << table->column(0)->data()->chunk(i)->length() << std::endl;
//            for (int64_t j = 0; j < table->column(0)->data()->chunk(i)->length(); j++) {
//                // Another simplification in this example is that we assume that there are
//                // no null entries, e.g. each row is fill with valid values.
//                int64_t valInt = col1->Value(j);
//                double valDouble = col2->Value(j);
//                std::cout << "Deserialized " << valInt << " and " << valDouble << std::endl;
//
//
//            }
//        }
//        std::cout << "The End" << std::endl;
//    }
//
//    {
//        std::cout << "Sel Test - Vector Input" << std::endl;
//        //input
//        auto testData = std::make_unique<int[]>(VECTORSIZE);
//        for(int i = 0; i < VECTORSIZE; i++) {
//            testData[i] = i + 1;
//        }
//        auto data(std::make_unique<std::array<int,VECTORSIZE>>());
//        std::copy(testData.get(),testData.get() + VECTORSIZE,(*data).begin());
//
//        auto dataVec(ArrayVector<int>(std::move(data)));
//        std::bitset<VECTORSIZE> selVector;
//
//        select_gt_pred(dataVec,7,10,selVector);
//        std::cout << selVector.count() << std::endl;
//    }
//
//    {
//        std::cout << "Folly futures - test 1" << std::endl;
//        //Buffer Pool Tests in conjunction with folly futures
//        using bpType = std::shared_ptr<BufferPool<int,ArrayVector<int>>>;
//        folly::Promise<bpType> p;
//        folly::Future<bpType> fut = p.getFuture();
//
//        auto bp = std::make_shared<BufferPool<int,ArrayVector<int>>>();
//
//        fut.then(askForBuf);
//
//        std::cout << "(Folly) Future chain made" << std::endl;
//
//        std::cout << "Fulfilling (Folly) Promise" << std::endl;
//        p.setValue(bp);
//        std::cout << "(Folly) Promise fulfilled" << std::endl;
//
//    }
//
//    {
//        std::cout << "Folly futures - promises - test 2" << std::endl;
//
//        std::vector<folly::Future<int>> futures;
//        for (int i = 0; i < 20; i++) {
//            //pretty much prepare input for compute function
//            futures.push_back(folly::makeFuture(i));
//        }
//
//        int threadsNo = 20;
//        std::shared_ptr<folly::Executor> e1 = std::make_shared<folly::CPUThreadPoolExecutor>(threadsNo);
//
//        for (auto& f : futures) {
//            f.then(e1.get(),compute);
//        }
//
//    }

//    {
//        //Works
//        std::cout << "Folly futures - bp test" << std::endl;
//        using bpType = std::shared_ptr<BufferPool<int,ArrayVector<int>>>;
//        auto bp = std::make_shared<BufferPool<int,ArrayVector<int>>>();
//
//        std::vector<folly::Future<bpType>> futures;
//        for (int i = 0; i < 20; i++) {
//            //pretty much prepare input for compute function
//            futures.push_back(folly::makeFuture(bp));
//        }
//
//        int threadsNo = 20;
//        std::shared_ptr<folly::Executor> e1 = std::make_shared<folly::CPUThreadPoolExecutor>(threadsNo);
//
//        for (auto& f : futures) {
//            f.then(e1.get(),askForBuf);
//        }
//
//    }

//    {
//        //Encapsulate buffer pool logic. Works.
//        std::cout << "Folly futures - encapsulated bp test" << std::endl;
//
//        std::vector<folly::Future<folly::Unit>> futures;
//        for (int i = 0; i < 20; i++) {
//            //pretty much prepare input for compute function
//            futures.push_back(folly::makeFuture());
//        }
//
//        int threadsNo = 20;
//        std::shared_ptr<folly::Executor> e1 = std::make_shared<folly::CPUThreadPoolExecutor>(threadsNo);
//
//        for (auto& f : futures) {
//            f.then(e1.get(),askForBuf_);
//        }
//
//    }

//    {
//        std::cout << "Folly futures - promises - test 3" << std::endl;
//        std::thread::id this_id = std::this_thread::get_id();
//        std::cout << "Dispatching thread: " << this_id << "\n";
//
//        std::vector<folly::Promise<int>> promises(1000);
//        std::vector<folly::Future<int>> futures;
//        for (auto& p : promises) {
//            futures.push_back(p.getFuture());
//        }
//
//        for (auto& f : futures) {
//            f.then(compute);
//        }
//
//        int id = 1;
//        for (auto& p : promises) {
//            p.setValue(id++);
//        }
//
//        //folly::collect(futures);
//    }

//    {
//        std::cout << "std futures - test 1" << std::endl;
//
//        std::vector<std::future<int>> futures;
//        for(int i = 0; i < 10; ++i) {
//            futures.push_back (std::async(std::launch::async, compute, i));
//        }
//
//        for(auto &e : futures)  {
//            std::cout << e.get() << std::endl;
//        }
//    }

//    {
//        std::cout << "Folly futures - async attempt" << std::endl;
////        //Buffer Pool Tests in conjunction with folly futures
////        using bpType = std::shared_ptr<BufferPool<int,ArrayVector<int>>>;
//
//        std::thread::id this_id = std::this_thread::get_id();
//        std::cout << "Dispatching thread: " << this_id << "\n";
//        folly::Promise<int> p;
//        auto fut = p.getFuture();
//
//        int threadsNo = 10;
//        std::shared_ptr<folly::Executor> e1 = std::make_shared<folly::CPUThreadPoolExecutor>(threadsNo);
//
//        fut.then(e1.get(),compute);
//
//        p.setValue(114);
////
////        auto bp = std::make_shared<BufferPool<int,ArrayVector<int>>>();
////
////        fut.then(askForBuf);
////
////        std::cout << "(Folly) Future chain made" << std::endl;
////
////        std::cout << "Fulfilling (Folly) Promise" << std::endl;
////        p.setValue(bp);
////        std::cout << "(Folly) Promise fulfilled" << std::endl;
//    }

    {
        CSVHelper csv = CSVHelper("/home/manolee/IdeaProjects/SQLPlanner/src/main/resources/schema/sales.csv",
                                  "/home/manolee/deleteme/data/sales.csv");

    }

//    {
//        std::cout << "Folly futures - test 3" << std::endl;
//
//        using bpType = std::shared_ptr<BufferPool<int,ArrayVector<int>>>;
//        auto bp = std::make_shared<BufferPool<int,ArrayVector<int>>>();
//
//
//        folly::Future<int> fut = askForBuf(bp);
//
//    }

//    {
//        lock_free_stack<int> s;
//        std::thread t1(push, &s);
//        std::thread t2(pop, &s);
//        t1.join();
//        t2.join();
//    }

//    {
//        lock_free_stack_hp<int> s;
//        std::thread t1(push_hp, &s);
//        std::thread t2(pop_hp, &s);
//        t1.join();
//        t2.join();
//    }

//    {
//        stack_sp<int> s;
//        std::thread t1(push_sp, &s);
//        std::thread t2(pop_sp, &s);
//        t1.join();
//        t2.join();
//    }


    {
        Catalog cat;
        cat.registerDataFolder("/home/manolee/deleteme/data");

        PlanHelper ph(cat);
        auto root = ph.parsePlan("/home/manolee/IdeaProjects/SQLPlanner/current.json");

        std::cout << root->print() << std::endl;

        std::cout << "End of exec" << std::endl;
    }

    return 0;
}