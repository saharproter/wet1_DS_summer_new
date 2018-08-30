#include "avlTree.h"
#include "library1.h"
#include <cstdlib>

class Company;
class Worker;

using namespace std;

/************************************************************/
//* Company
/************************************************************/
class Company{
    int companyID;
    int numOfWorkers;
    AVLtree<Worker, int>* CompanyWorkersByID;
    AVLtree<Worker, int>* CompanyWorkersByRank;
    int bestWorkerID;
    int bestWorkerRank;
public:
    Company (int companyID) : companyID(companyID), numOfWorkers(0), bestWorkerID(-1), bestWorkerRank(0) {
        CompanyWorkersByID =  new AVLtree<Worker, int>;
        CompanyWorkersByRank =  new AVLtree<Worker, int>;
    }
    Company(int companyID , int numOfWorkers , AVLtree<Worker, int>* CompanyWorkersByID , AVLtree<Worker, int>* CompanyWorkersByRank , int bestWorkerID , int bestWorkerRank){
        this->companyID = companyID;
        this->numOfWorkers = numOfWorkers;
        this->CompanyWorkersByID = CompanyWorkersByID;
        this->CompanyWorkersByRank = CompanyWorkersByRank;
        this->bestWorkerID = bestWorkerID;
        this->bestWorkerRank = bestWorkerRank;

    }
    ~Company() {
        //CompanyWorkersByRank->removeAllData();
        //CompanyWorkersByID->removeAllData();
        delete CompanyWorkersByRank;
        delete CompanyWorkersByID;
    }
    int getCompanyID() const{
        return companyID;
    }
    int getCompanyNumOfWorkers(){
        return numOfWorkers;
    }
    AVLtree<Worker, int>* getWorkersByIDTree(){
        return CompanyWorkersByID;
    }
    AVLtree<Worker, int>* getWorkersByRankTree(){
        return CompanyWorkersByRank;
    }
    int getBestWorkerID(){
        return bestWorkerID;
    }
    int getBestWorkerRank(){
        return bestWorkerRank;
    }
    void AddWorker(){
        numOfWorkers++;
    }
    void substructWorker(){
        numOfWorkers--;
    }
    void setCompanyID(int newID){
        companyID = newID;
    }
    void setBestWorkerID(int newID){
        bestWorkerID = newID;
    }
    void setBestWorkerRank(int newRank){
        bestWorkerRank = newRank;
    }
    void setNumOfWorkers(int n){
        numOfWorkers = n;
    }
    void setWorkerByIDTree(AVLtree<Worker, int>* t){
        delete CompanyWorkersByID;
        CompanyWorkersByID = t;
    }
    void setWorkerByRankTree(AVLtree<Worker, int>* t){
        delete CompanyWorkersByRank;
        CompanyWorkersByRank = t;
    }
};
class compareCompanyIDs: public IsLess<Company>{
public:
    bool operator() (Company* company1, Company* company2) const {
        return ( company1->getCompanyID() < company2->getCompanyID() );
    }
};

class compareCompaniesByKeyIDs : public IsLessThanKey<Company, int>{
public:
    bool operator() (Company* c, int id) const {
        return c->getCompanyID() < id;
    }
    bool operator() (int id, Company* c) const {
        return id < c->getCompanyID();
    }
};

/************************************************************/
//* Worker
/************************************************************/
class Worker{
    int workerID;
    int rank;
    Company* company;

public:
    Worker(int workerID, int rank, Company* company = NULL) : workerID(workerID), rank(rank), company(company){}
    Worker( const Worker &w) : workerID(w.workerID), rank(w.rank), company(w.company) {}

    int getWorkerID() const{
        return workerID;
    }
    int getWorkerRank(){
        return rank;
    }
    Company* getCompany(){
        return company;
    }
    int getWorkerCompanyID(){
        if(company)
            return company->getCompanyID();
        return -1;
    }
    void setWorkerID(int newID){
        workerID = newID;
    }
    void setWorkerRank(int newRank){
        rank = newRank;
    }
    void setCompany(Company* newCompany){
        company = newCompany;
    }
};
class compareWorkerIDs: public IsLess<Worker>{
public:
    bool operator() (Worker*worker1, Worker* worker2) const {
        return ( worker1->getWorkerID() < worker2->getWorkerID() );
    }
};
class compareWorkerRanks: public IsLess<Worker>{
public:
    bool operator() (Worker*worker1, Worker* worker2) const {
        return  worker1->getWorkerRank() < worker2->getWorkerRank() ? true :
        worker1->getWorkerRank() > worker2->getWorkerRank() ? false :
        (worker1->getWorkerID() > worker2->getWorkerID() ? true : false);
    }
};
class compareWorkersByKeyIDs : public IsLessThanKey<Worker, int>{
public:
    bool operator() (Worker* w, int id) const {
        return w->getWorkerID() < id;
    }
    bool operator() (int id, Worker* w) const {
        return id < w->getWorkerID();
    }
};
/************************************************************/
//* DataStructures
/************************************************************/
class DataSturcture{
    AVLtree<Company, int>* Companies;
    AVLtree<Worker, int>* WorkersByID;
    AVLtree<Worker, int>* WorkersByRank;
    int bestWorkerID;
    int bestWorkerRank;

public:

    DataSturcture() : bestWorkerID(-1), bestWorkerRank(0){
        Companies = new AVLtree<Company, int>;
        WorkersByID = new AVLtree<Worker, int>;
        WorkersByRank = new AVLtree<Worker, int>;
    }
    ~DataSturcture(){
        Companies->removeAllData();
        WorkersByID->removeAllData();
        delete Companies;
        delete WorkersByID;
        delete WorkersByRank;
    }
    int getBestWorkerID(){
        return bestWorkerID;
    }
    int getBestWorkerRank(){
        return bestWorkerRank;
    }
    void setBestWorkerID(int newID){
        bestWorkerID = newID;
    }

    void setBestWorkerRank(int newRank){
        bestWorkerRank = newRank;
    }
    /************************************************************/
    StatusType addWorker(int workerID, int rank){

        if ( workerID <= 0 || rank <= 0){
            return INVALID_INPUT;
        }
        if (WorkersByID->searchElement(workerID, compareWorkersByKeyIDs()) != NULL){
            return FAILURE;
        }
        Worker * worker = new Worker(workerID, rank);

        if (worker == NULL)
        {
            return ALLOCATION_ERROR;
        }

        StatusType status = WorkersByID->Insert(worker, compareWorkerIDs());
        if ( status != SUCCESS){
            delete worker;
            return ALLOCATION_ERROR;
        }

        status = WorkersByRank->Insert(worker, compareWorkerRanks());
        if ( status != SUCCESS){
            delete worker;
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }

    /************************************************************/
    StatusType addCompany(int companyID){

        if (companyID <= 0)
        {
            return INVALID_INPUT;
        }

        if (Companies->searchElement(companyID, compareCompaniesByKeyIDs()) != NULL){
            return FAILURE;
        }
        Company * company = new Company(companyID);

        if (company == NULL)
        {
            return ALLOCATION_ERROR;
        }

        StatusType status = Companies->Insert(company, compareCompanyIDs());
        if ( status != SUCCESS){
            delete company;
            return ALLOCATION_ERROR;
        }

        return SUCCESS;
    }

    /************************************************************/
    StatusType addworkerToCompany(int workerID, int companyID){

        if ( workerID <= 0 || companyID <= 0){
            return INVALID_INPUT;
        }

        Company* company = Companies->searchElement(companyID, compareCompaniesByKeyIDs());

        if (company == NULL){
            return FAILURE;
        }

        Worker* worker = WorkersByID->searchElement(workerID, compareWorkersByKeyIDs());

        if (worker == NULL){
            return FAILURE;
        }

        // Add worker with no company yet.
        if( worker->getCompany() == NULL){
            company->getWorkersByIDTree()->Insert(worker, compareWorkerIDs());
            company->getWorkersByRankTree()->Insert(worker, compareWorkerRanks());
            company->AddWorker();
            worker->setCompany(company);
        }

        else {
            // Remove worker from previus company and add to the new one.
            (worker->getCompany())->getWorkersByIDTree()->Remove(worker, compareWorkerIDs());
            (worker->getCompany())->getWorkersByRankTree()->Remove(worker, compareWorkerRanks());
            (worker->getCompany())->substructWorker();

            worker->setCompany(company);

            company->getWorkersByIDTree()->Insert(worker, compareWorkerIDs());
            company->getWorkersByRankTree()->Insert(worker, compareWorkerRanks());
            company->AddWorker();
        }
        //Update company's best workerID and rank.
        company->setBestWorkerID(((company->getWorkersByRankTree())->max())->getWorkerID());
        company->setBestWorkerRank(((company->getWorkersByRankTree())->max())->getWorkerRank());

        return SUCCESS;
    }

    /************************************************************/
    StatusType removeWorker(int workerID){

        if ( workerID <= 0){
            return INVALID_INPUT;
        }

        Worker* worker = WorkersByID->searchElement(workerID, compareWorkersByKeyIDs());
        if (worker == NULL){
            return FAILURE;
        }
        Company* company = worker->getCompany();

        WorkersByID->Remove(worker, compareWorkerIDs());
        WorkersByRank->Remove(worker, compareWorkerRanks());
        if(!company)
            return SUCCESS;
        company->getWorkersByIDTree()->Remove(worker, compareWorkerIDs());
        company->getWorkersByRankTree()->Remove(worker, compareWorkerRanks());
        delete worker;
        company->substructWorker();

        //Update company's best worker.
        if(company->getWorkersByRankTree()->isEmpty()){
            company->setBestWorkerID(-1);
            company->setBestWorkerRank(-1);
            return SUCCESS;
        }
        company->setBestWorkerID(((company->getWorkersByRankTree())->max())->getWorkerID());
        company->setBestWorkerRank(((company->getWorkersByRankTree())->max())->getWorkerRank());

        return SUCCESS;
    }

    /************************************************************/
    StatusType mergeCompanies(int companyID1, int companyID2, int minimalRank){

        Worker** t_arr1 , **t_arr2;
        int n1 , n2;

        if ( companyID1 <= 0 || companyID2 <= 0 || minimalRank <= 0 || companyID1 == companyID2){
            return INVALID_INPUT;
        }

        // company1 is not found.
        Company* company1 = Companies->searchElement(companyID1, compareCompaniesByKeyIDs());
        if ( company1 == NULL){
            return FAILURE;
        }

        // company2 is not found.
        Company* company2 = Companies->searchElement(companyID2, compareCompaniesByKeyIDs());
        if ( company2 == NULL){
            return FAILURE;
        }

        //  merge workersById array
        company1->getWorkersByIDTree()->InorderArray(t_arr1 , &n1);
        company2->getWorkersByIDTree()->InorderArray(t_arr2 , &n2);

        Worker* arrayById[n1+n2] , *arrayByRank[n1+n2];
        int size;

        mergeWorkerArr(t_arr1 , t_arr2 , n1 , n2 , arrayById , &size , compareWorkerIDs() , minimalRank);

        delete[] t_arr1;
        delete[] t_arr2;

        //  merge workersByRank array
        company1->getWorkersByRankTree()->InorderArray(t_arr1 , &n1);
        company2->getWorkersByRankTree()->InorderArray(t_arr2 , &n2);
        mergeWorkerArr(t_arr1 , t_arr2 , n1 , n2 , arrayByRank , &size , compareWorkerRanks() , minimalRank);

        delete[] t_arr1;
        delete[] t_arr2;

        Company *newCompany;

        // set which company remains and removes the other
        if(n1 > n2 || (n1==n2 && company1->getCompanyID() < company2->getCompanyID())) {
            newCompany = company1;
            Companies->Remove(company2 , compareCompanyIDs());
            delete company2;
        }
        else {
            newCompany = company2;
            Companies->Remove(company1, compareCompanyIDs());
            delete company1;
        }

        // set all remaining workers to point merged company
        for(int i=0;i<size;i++) {
            arrayById[i]->setCompany(newCompany);
            arrayByRank[i]->setCompany(newCompany);
        }

        //create 2 new merged ID and Rank trees from arrays
        AVLtree<Worker , int>* workersById = new AVLtree<Worker , int>(arrayById , size);
        AVLtree<Worker , int>* workersByRank = new AVLtree<Worker , int>(arrayByRank , size);

        // updating company's workrers trees
        newCompany->setWorkerByIDTree(workersById);
        newCompany->setWorkerByRankTree(workersByRank);
        newCompany->setNumOfWorkers(size);

        // updating company's best worker
        newCompany->setBestWorkerID(newCompany->getWorkersByRankTree()->max()->getWorkerID());
        newCompany->setBestWorkerRank(newCompany->getWorkersByRankTree()->max()->getWorkerRank());

        return SUCCESS;
    }
    void mergeWorkerArr(Worker** arr1 , Worker** arr2 , int n1 , int n2 , Worker** arr3 , int* n3 , const IsLess<Worker>& less , int minRank){

        int a=0,b=0,c=0;
        *n3 = 0;
        while(a<n1 && b<n2){
            if(!less(arr2[b] , arr1[a])) {
                if(arr1[a]->getWorkerRank() >= minRank) {
                    arr3[c++] = arr1[a++];
                    (*n3)++;
                }else {
                    arr1[a++]->setCompany(NULL);
                }
            }
            else {
                if(arr2[a]->getWorkerRank() >= minRank) {
                    arr3[c++] = arr2[b++];
                    (*n3)++;
                } else{
                    arr2[b++]->setCompany(NULL);
                }
            }
        }
        while(a < n1) {
            if(arr1[a]->getWorkerRank() >= minRank) {
                arr3[c++] = arr1[a++];
                (*n3)++;
            }else{
                arr1[a++]->setCompany(NULL);
            }
        }
        while(b < n2) {
            if(arr2[b]->getWorkerRank() >= minRank) {
                arr3[c++] = arr2[b++];
                (*n3)++;
            }else{
                arr2[b++]->setCompany(NULL);
            }
        }
    }
    /************************************************************/
    StatusType changeRank(int workerID, int newRank){

        if ( workerID <=0 || newRank <= 0){
            return INVALID_INPUT;
        }

        Worker* worker = WorkersByID->searchElement(workerID, compareWorkersByKeyIDs());
        if ( worker == NULL ){
            return FAILURE;
        }
        Company* company = Companies->searchElement(worker->getWorkerCompanyID(), compareCompaniesByKeyIDs());

        // Remover worker with workerID with old rank.
        WorkersByID->Remove(worker, compareWorkerIDs());
        WorkersByRank->Remove(worker, compareWorkerRanks());
        if(company) {
            company->getWorkersByIDTree()->Remove(worker, compareWorkerIDs());
            company->getWorkersByRankTree()->Remove(worker, compareWorkerRanks());
        }
        worker->setWorkerRank(newRank);

        // Insert woker with workerID with new rank.
        WorkersByID->Insert(worker, compareWorkerIDs());
        WorkersByRank->Insert(worker, compareWorkerRanks());
        if(company) {
            company->getWorkersByIDTree()->Insert(worker, compareWorkerIDs());
            company->getWorkersByRankTree()->Insert(worker, compareWorkerRanks());

            //Update company's best worker.
            company->setBestWorkerID(((company->getWorkersByRankTree())->max())->getWorkerID());
            company->setBestWorkerRank(((company->getWorkersByRankTree())->max())->getWorkerRank());

        }
        bestWorkerID = WorkersByRank->max()->getWorkerID();
        bestWorkerRank = WorkersByRank->max()->getWorkerRank();
        return SUCCESS;
    }

    /************************************************************/
    StatusType getBestWorker(int companyID, int *workerID){

        if ( companyID ==  0 || workerID == NULL){
            return INVALID_INPUT;
        }

        if ( companyID < 0){
            if(WorkersByID->isEmpty())
                *workerID = -1;
            else
                *workerID = bestWorkerID;
            return SUCCESS;
        }

        // No workers to any company
        if ( Companies->isEmpty()){
            return FAILURE;
        }

        Company* company = Companies->searchElement(companyID, compareCompaniesByKeyIDs());
        if ( company == NULL){
            return FAILURE;
        }

        if( !(company->getWorkersByIDTree()->isEmpty())){
            *workerID = company->getBestWorkerID();
        }
        else {
            *workerID = -1;
        }

        return SUCCESS;
    }
    /************************************************************/
    StatusType getCompanyWorkersByRank (int companyID, int **workers, int *numOfWorkers){

        if ( companyID == 0 || workers == NULL || numOfWorkers == NULL){
            return INVALID_INPUT;
        }
        if( companyID < 0 ){
            if ( WorkersByRank->max() == 0){
                *workers = NULL;
                *numOfWorkers = 0;
            }
            else {
                Worker** workerArr = NULL;
                WorkersByRank->InorderArray(workerArr, numOfWorkers);
                (*workers) = (int*)malloc((*numOfWorkers)*sizeof(int));
                if(numOfWorkers != NULL && *numOfWorkers >0 && *workers ==  NULL){
                    return ALLOCATION_ERROR;
                }
                for (int i = 0; i < *numOfWorkers; i++){
                    (*workers)[i] = workerArr[*numOfWorkers-i-1]->getWorkerID();
                }
                delete[] workerArr;
            }
            return SUCCESS;
        }
        if ( Companies->isEmpty()){
            return FAILURE;
        }

        Company* company = Companies->searchElement(companyID, compareCompaniesByKeyIDs());
        if ( company == NULL){
            return FAILURE;
        }
        if (company->getWorkersByIDTree()->isEmpty()){
            *workers = NULL;
            *numOfWorkers = 0;
            return SUCCESS;
        }
        Worker** workerArr = NULL;
        company->getWorkersByRankTree()->InorderArray(workerArr, numOfWorkers);
        (*workers) = (int*)malloc((*numOfWorkers)*sizeof(int));
        if(numOfWorkers != NULL && *numOfWorkers >0 && *workers ==  NULL){
            return ALLOCATION_ERROR;
        }
        for ( int i = 0 ; i< *numOfWorkers; i++){
            (*workers)[i] = workerArr[*numOfWorkers-i-1]->getWorkerID();
        }
        delete[] workerArr;
        return SUCCESS;
    }
};

/************************************************************/
//* Init
/************************************************************/
void* init(){
    DataSturcture* DS = new DataSturcture();
    return DS;
}

/************************************************************/
//* addWorker
/************************************************************/
StatusType addWorker(void *DS, int workerID, int rank){
    if (DS == NULL){
        return INVALID_INPUT;
    }
    DataSturcture* DSnew = static_cast<DataSturcture*>(DS);
    StatusType status = DSnew->addWorker(workerID, rank);
    return status;
}

/************************************************************/
//* addCompany
/************************************************************/
StatusType addCompany(void *DS, int companyID){
    if (DS == NULL){
        return INVALID_INPUT;
    }
    DataSturcture* DSnew = static_cast<DataSturcture*>(DS);
    StatusType status = DSnew->addCompany(companyID);
    return status;
}

/************************************************************/
//* addworkerToCompany
/************************************************************/
StatusType addworkerToCompany(void *DS, int workerID, int companyID){
    if (DS == NULL){
        return INVALID_INPUT;
    }
    DataSturcture* DSnew = static_cast<DataSturcture*>(DS);
    StatusType status = DSnew->addworkerToCompany(workerID, companyID);
    return status;
}

/************************************************************/
//* removeWorker
/************************************************************/
StatusType removeWorker(void *DS, int workerID){
    if (DS == NULL){
        return INVALID_INPUT;
    }
    DataSturcture* DSnew = static_cast<DataSturcture*>(DS);
    StatusType status = DSnew->removeWorker(workerID);
    return status;
}

/************************************************************/
//* mergeCompanies
/************************************************************/
StatusType mergeCompanies(void *DS, int companyID1, int companyID2, int minimalRank){
    if (DS == NULL){
        return INVALID_INPUT;
    }
    DataSturcture* DSnew = static_cast<DataSturcture*>(DS);
    StatusType status = DSnew->mergeCompanies(companyID1, companyID2, minimalRank);
    return status;
}

/************************************************************/
//* changeRank
/************************************************************/
StatusType changeRank(void *DS, int workerID, int newRank){
    if (DS == NULL){
        return INVALID_INPUT;
    }
    DataSturcture* DSnew = static_cast<DataSturcture*>(DS);
    StatusType status = DSnew->changeRank(workerID, newRank);
    return status;
}

/************************************************************/
//* getBestWorker
/************************************************************/
StatusType getBestWorker(void *DS, int companyID, int *workerID){
    if (DS == NULL){
        return INVALID_INPUT;
    }
    DataSturcture* DSnew = static_cast<DataSturcture*>(DS);
    StatusType status = DSnew->getBestWorker(companyID, workerID);
    return status;
}

/************************************************************/
//* getCompanyWorkersByRank
/************************************************************/
StatusType getCompanyWorkersByRank (void *DS, int companyID, int **workers, int *numOfWorkers){
    if (DS == NULL){
        return INVALID_INPUT;
    }
    DataSturcture* DSnew = static_cast<DataSturcture*>(DS);
    StatusType status = DSnew->getCompanyWorkersByRank(companyID, workers, numOfWorkers);
    return status;
}

/***********************************************************/
//* Quit
/***********************************************************/

void quit(void** DS){
    if(DS == NULL || *DS == NULL)
        return;
    DataSturcture** DSnew = (DataSturcture**)DS;
    delete *DSnew;
    *DS = NULL;
}



