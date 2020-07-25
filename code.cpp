#include <bits/stdc++.h>
#include <map>
using namespace std;
INT CAPACITY = 1000;
//stores a unique id for every order placed and keeps a count of orders
int SellId=0;
int BuyId=0;
//we store the status of every order placed(pending/excecuted/cancelled/converted ) in two maps
map<int, string> Buy;
map<int, string> Sell;

struct BuyPricePoint{
    //int hours;
    //int minutes;
    int quantity;
    float price;
    struct BuyPricePoint* next;
    int id;
};

struct SellPricePoint{
    //int hours;
    //int minutes;
    int quantity;
    float price;
    struct SellPricePoint* next;
    int id;
};


struct SellPricePoint ArraySell[CAPACITY];
struct BuyPricePoint ArrayBuy[CAPACITY];
int ArraySellNo=0;
int ArrayBuyNo=0;

//Create a new struct to add in the array of linked list
struct SellPricePoint* NewSellStruct(int n, float p){
    struct SellPricePoint* temp=(struct SellPricePoint*)calloc(1,sizeof(struct SellPricePoint));;
    temp->quantity=n;
    temp->price=p;
    SellId++;
    temp->id=SellId;
    temp->next=NULL;
    return temp;
}

//Create a new struct to add in the array of linked list
struct BuyPricePoint* NewBuyStruct(int n, float p){
    struct BuyPricePoint* temp=(struct BuyPricePoint*)calloc(1,sizeof(struct BuyPricePoint));
    temp->quantity=n;
    temp->price=p;
    BuyId++;
    temp->id=BuyId;
    temp->next=NULL;
    return temp;
}

//shifting all linked lists after ith position to accomodate a linked list at ith position.
// this is done so that the orderbook is sorted at all times
void BuyShiftRight(int i){
    //shift all elements from i to one position right
    for(int j=ArrayBuyNo-1;j>=i;j--){
        ArrayBuy[j+1]=ArrayBuy[j];
    }
}

void SellShiftRight(int i){
    //shift all elements from i to one position right
    for(int j=ArraySellNo-1;j>=i;j--){
        ArraySell[j+1]=ArraySell[j];
    }
}

//shifting all linked lists after ith position to the left when an entry is canceled or executed.
// this is done so that the orderbook does not store redundant entries, keeping in mind, Space and time complexity both
//(time complexity during future transactions)
void BuyShiftLeft(int i){
    //shift all elements from i to one position left
    for(int j=i;j<ArrayBuyNo-1;j++){
        ArrayBuy[j]=ArrayBuy[j+1];
    }
}

void SellShiftLeft(int i){
    //shift all elements from i to one position left
    for(int j=i;j<ArraySellNo-1;j++){
        ArraySell[j]=ArraySell[j+1];
    }
}

//if the Buy/Sell order does not satisfy the outstanding Sell/Buy orders,
//they are stored in the respective array of the orderbook 
void PlaceLimitBuyOrder(int n,float p){
    struct BuyPricePoint* temp=NewBuyStruct(n,p);
    //if the Buy Array is empty, then add in the first position
    if(ArrayBuyNo==0) {
        ArrayBuy[0]= *temp;
        ArrayBuyNo++;
        cout<<"buy order placed"<<endl;
        cout<<"your order id is "<<ArrayBuy[0].id<<endl;
    }
    else{ 
        int flag=0;
        for(int i=0;i<ArrayBuyNo;i++){
            //if an entry of the same price point exists, then add to the end of the linked list
            if(ArrayBuy[i].price==p){
                struct BuyPricePoint* temp1 = new(struct BuyPricePoint);
                temp1= &ArrayBuy[i];
                while(temp1->next != NULL){
                    temp1=(temp1->next);
                }
                temp1->next= temp;
                flag=1;
                cout<<"buy order placed"<<endl;
                cout<<"your order id is "<<temp->id<<endl;
                break;
            }
            //otherwise add it to array such that the orderbook is always sorted
            else if(ArrayBuy[i].price<p){
                //add the struct before this point
                BuyShiftRight(i);
                ArrayBuy[i]= *temp;
                flag=1;
                ArrayBuyNo++;
                cout<<"buy order placed"<<endl;
                cout<<"your order id is "<<temp->id<<endl;
                break;
            }
        }
        //add to the end of the array if price is smaller than any entry 
        if(flag==0){
            ArrayBuy[ArrayBuyNo]= *temp;
            ArrayBuyNo++;
            cout<<"buy order placed"<<endl;
            cout<<"your order id is "<<temp->id<<endl;
        }
    }
    //update the status of the order as pending
    Buy.insert({temp->id,"pending"});
}

void PlaceLimitSellOrder(int n,float p){
    struct SellPricePoint* temp=NewSellStruct(n,p);
    //if the Buy Array is empty, then add in the first position
    if(ArraySellNo==0) {
        ArraySell[0]= *temp;
        ArraySellNo++;
        cout<<"sell order placed"<<endl;
        cout<<"your order id is "<<temp->id<<endl;
    }
    else{ 
        int flag=0;
        for(int i=0;i<ArraySellNo;i++){
            //if an entry of the same price point exists, then add to the end of the linked list
            if(ArraySell[i].price==p){
                struct SellPricePoint* temp1 = new(struct SellPricePoint);
                temp1= &ArraySell[i];
                while(temp1->next != NULL){
                    temp1=(temp1->next);
                }
                temp1->next= temp;
                flag=1;
                cout<<"sell order placed"<<endl;
                cout<<"your order id is "<<temp->id<<endl;
                break;
            }
            //otherwise add it to array such that the orderbook is always sorted
            else if(ArraySell[i].price>p){
                //add the struct before this point
                SellShiftRight(i);
                ArraySell[i]= *temp;
                flag=1;
                ArraySellNo++;
                cout<<"sell order placed"<<endl;
                cout<<"your order id is "<<temp->id<<endl;
                break;
            }
        }
        //add to the end of the array if price is greater than any entry
        if(flag==0){
            ArraySell[ArraySellNo]= *temp;
            ArraySellNo++;
            cout<<"sell order placed"<<endl;
            cout<<"your order id is "<<temp->id<<endl;
        }
    }
    //update the status of the order as pending
    Sell.insert({temp->id,"pending"});
}

//if the price matches with best offer, start matching from the highest possible order 
//which matches in the sell array to the lower ones
void MatchLimitBuy(int n, float p){
    int i=0;
    while(n!=0 && ArraySell[i].price<=p && ArraySellNo>0){
        struct SellPricePoint* temp1 = new(struct SellPricePoint); 
        temp1= &ArraySell[i];
        int N=0;
        //finding total orders at a particular price
        do{
            N+=temp1->quantity;
            temp1=(temp1->next);
        }while(temp1 != NULL);
        //if total orders at that position's linked list is greater than required orders,
        //match n orders from the linked lists in fifo order
        if(n<=N) {
            temp1= &ArraySell[i];
            while(n!=0){
                if(n<=temp1->quantity) {
                    temp1->quantity=temp1->quantity-n;
                    if(temp1->quantity==0){
                        Sell.erase(temp1->id);
                        Sell.insert({temp1->id,"executed"});
                    }
                    n=0;
                    BuyId++;
                    Buy.insert({BuyId,"executed"});
                    cout<<"buy order excecuted"<<endl;
                    cout<<"your order id is "<<BuyId<<endl;
                    break;
                }
                else if(n>temp1->quantity){
                    n=n - temp1->quantity;
                    temp1->quantity=0;
                    Sell.erase(temp1->id);
                    Sell.insert({temp1->id,"executed"});
                }
                if(n==0) break;
                temp1=(temp1->next);
            }
            break;
        }
        //if the required no., is more than number of orders at that position then 
        //exaust orders at that position and go to the next one
        else if(n>N){
            n=n - N;
            temp1= &ArraySell[i];
            while(temp1->next != NULL){
            Sell.erase(temp1->id);
            Sell.insert({temp1->id,"executed"});
            temp1=(temp1->next);
            }
            if(ArraySellNo>1) SellShiftLeft(i);
            else{ArraySell[0].price=0;
            ArraySell[0].quantity=0;
            ArraySell[0].next= NULL;
            }
            ArraySellNo--;
        }
        if(n==0) break;
    }
    if(n!=0) PlaceLimitBuyOrder(n,p);
}

//if the price matches with best bid, start matching from the least possible bid 
//which matches in the buy array to the higher ones
void MatchLimitSell(int n, float p){
    int i=0;
    while(n!=0 && ArrayBuy[i].price>=p && ArrayBuyNo>0){
        struct BuyPricePoint* temp1= new(struct BuyPricePoint); 
        temp1= &ArrayBuy[i];
        int N=0;
        //finding total orders at a particular price
        do{
            N+=temp1->quantity;
            temp1=(temp1->next);
        }while(temp1!= NULL);
        //if total orders at that position's linked list is greater than required orders,
        //match n orders from the linked lists in fifo order
        if(n<=N) {
            temp1= &ArrayBuy[i];
            while(n!=0){
                if(n<=temp1->quantity) {
                    temp1->quantity=temp1->quantity-n;
                    if(temp1->quantity==0){
                        Buy.erase(temp1->id);
                        Buy.insert({temp1->id,"executed"});
                    }
                    n=0;
                    SellId++;
                    Sell.insert({SellId,"executed"});
                    cout<<"sell order executed"<<endl;
                    cout<<"your order id is "<<SellId<<endl;
                    break;
                }
                else if(n>temp1->quantity){
                    n=n - temp1->quantity;
                    temp1->quantity=0;
                    Buy.erase(temp1->id);
                    Buy.insert({temp1->id,"executed"});
                }
                if(n==0) break;
                temp1=(temp1->next);
            }
            break;
        }
        //if the required no., is more than number of orders at that position then 
        //exaust orders at that position and go to the next one
        else if(n>N){
            n=n - N;
            temp1= &ArrayBuy[i];
            while(temp1->next != NULL){
            Buy.erase(temp1->id);
            Buy.insert({temp1->id,"executed"});
            temp1=(temp1->next);
            }
            if(ArrayBuyNo > 1) {
                BuyShiftLeft(i);
            }
            else{ ArrayBuy[0].price=0;
            ArrayBuy[0].quantity=0;
            ArrayBuy[0].next=NULL;
            }
            ArrayBuyNo--;
        }
        if(n==0) break;
    }
    if(n!=0) {PlaceLimitSellOrder(n,p);
    }
}

//start matching from the best possible offer 
void PlaceMarketBuyOrder(int n){
    int i=0;
    while(n>0){
        struct SellPricePoint* temp1 = new (struct SellPricePoint);
        temp1= &ArraySell[i];
        int N=0;
        //finding total orders at a particular price
        do{
            N+=temp1->quantity;
            temp1=(temp1->next);
        }while(temp1 != NULL);
        //if total orders at that position's linked list is greater than required orders,
        //match n orders from the linked lists in fifo order
        if(n<=N) {
            temp1= &ArraySell[i];
            while(n!=0){
                if(n < temp1->quantity) {
                    temp1->quantity=temp1->quantity-n;
                    n=0;
                    BuyId++;
                    Buy.insert({BuyId,"executed"});
                    cout<<"buy order excecuted"<<endl;
                    cout<<"your order id is "<<BuyId<<endl;
                    break;
                }
                else if(n>=temp1->quantity){
                    n=n - temp1->quantity;
                    if(n==0){
                        BuyId++;
                        Buy.insert({BuyId,"executed"});
                        cout<<"buy order excecuted"<<endl;
                        cout<<"your order id is "<<BuyId<<endl;
                    }
                    temp1->quantity=0;
                    Sell.erase(temp1->id);
                    Sell.insert({temp1->id,"executed"});
                }
                if(n==0) break;
                temp1=(temp1->next);
            }
            break;
        }
        //if the required no., is more than number of orders at that position then 
        //exaust orders at that position and go to the next one
        else if(n>N){
            n=n - N;
            temp1= &ArraySell[i];
            do{
                Sell.erase(temp1->id);
                Sell.insert({temp1->id,"executed"});
                temp1=(temp1->next);
            }while(temp1 != NULL);
            if(ArraySellNo>1) SellShiftLeft(i);
            else{ArraySell[0].price=0;
            ArraySell[0].quantity=0;
            ArraySell[0].next= NULL;
            }
            ArraySellNo--;
        }
        if(n==0) break;
    }
}

//start matching orders from the best bid
void PlaceMarketSellOrder(int n){
    int i=0;
    while(n>0){
        struct BuyPricePoint* temp1= new (struct BuyPricePoint);
        temp1= &ArrayBuy[i];
        int N=0;
        //finding total orders at a particular price
        do{
            N+=temp1->quantity;
            temp1=(temp1->next);
        }while(temp1 != NULL);
        //if total orders at that position's linked list is greater than required orders,
        //match n orders from the linked lists in fifo order
        if(n<=N) {
            temp1= &ArrayBuy[i];
            while(n!=0){
                if(n<temp1->quantity) {
                    temp1->quantity=temp1->quantity-n;
                    if(temp1->quantity==0){
                        Buy.erase(temp1->id);
                        Buy.insert({temp1->id,"executed"});
                    }
                    n=0;
                    SellId++;
                    Sell.insert({SellId,"executed"});
                    cout<<"sell order excecuted"<<endl;
                    cout<<"your order id is "<<SellId<<endl;
                    break;
                }
                else if(n>=temp1->quantity){
                    n=n - temp1->quantity;
                    if(n==0){
                        SellId++;
                        Sell.insert({SellId,"executed"});
                        cout<<"sell order excecuted"<<endl;
                        cout<<"your order id is "<<SellId<<endl;
                    }
                    temp1->quantity=0;
                    Buy.erase(temp1->id);
                    Buy.insert({temp1->id,"executed"});
                }
                if(n==0) break;
                temp1=(temp1->next);
            }
            break;
        }
        //if the required no., is more than number of orders at that position then 
        //exaust orders at that position and go to the next one
        else if(n>N){
            n=n - N;
            temp1= &ArrayBuy[i];
            do{
                Buy.erase(temp1->id);
                Buy.insert({temp1->id,"executed"});
                temp1=(temp1->next);
            }while(temp1->next != NULL);
            if(ArrayBuyNo > 1) BuyShiftLeft(i);
            else{ ArrayBuy[0].price=0;
            ArrayBuy[0].quantity=0;
            ArrayBuy[0].next=NULL;
            }
            ArrayBuyNo--;
        }
        if(n==0) break;
    }
}

void BuyLimitToMarket(int i){
    int flag=0;
    for(int j=0;j<ArrayBuyNo;j++){
        struct BuyPricePoint* temp= new(struct BuyPricePoint);
        temp=&ArrayBuy[j];
        if(temp->id==i && temp->next==NULL){
            PlaceMarketBuyOrder(temp->quantity);
            cout<<"order with id no. "<<i<<" is converted."<<endl;
            BuyShiftLeft(i);
            ArrayBuyNo--;
            flag=1;
            break;
        }
        else{ 
            do{
            if(flag==1) break;
            if(temp->id==i){
                PlaceMarketBuyOrder(temp->quantity);
                flag=1;
                cout<<"order with id no. "<<i<<" is converted."<<endl;
                break;
            }
            if(flag==1) break;
            temp=(temp->next);
        }while(temp!=NULL);
    }
        if(flag==1) break;
    }
    if(flag==0) cout<<"id not found"<<endl;
}

void SellLimitToMarket(int i){
    int flag=0;
    for(int j=0;j<ArraySellNo;j++){
        struct SellPricePoint* temp= new(struct SellPricePoint);
        temp=&ArraySell[j];
        if(temp->id==i && temp->next==NULL){
            PlaceMarketSellOrder(temp->quantity);
            cout<<"order with id no. "<<i<<" is cancelled."<<endl;
            SellShiftLeft(i);
            ArraySellNo--;
            flag=1;
            break;
        }
        else{
        do{
            if(flag==1) break;
            if(temp->id==i){
                PlaceMarketSellOrder(temp->quantity);
                flag=1;
                cout<<"order with id no. "<<i<<" is cancelled."<<endl;
                break;
            }
            if(flag==1) break;
            temp=(temp->next);
        }while(temp!=NULL);
    }
        if(flag==1) break;
    }
    if(flag==0) cout<<"id not found"<<endl;
}


//when an order is cancelled, we look for the id in each linked list of the Array
//and if it was the only struct in the linked list at that position, shift all the elements beyond it to left 
void BuyCancel(int i){
    int flag=0;
    for(int j=0;j<ArrayBuyNo;j++){
        struct BuyPricePoint* temp= new(struct BuyPricePoint);
        temp=&ArrayBuy[j];
        if(temp->id==i && temp->next==NULL){
            BuyShiftLeft(i);
            ArrayBuyNo--;
            flag=1;
            Sell.erase(i);
            Sell.insert({i,"cancelled"});
            cout<<"order with id no. "<<i<<" is cancelled."<<endl;
        }
        else{
        do{
            if(flag==1) break;
            if(temp->id==i){
                flag=1;
                Sell.erase(i);
                Sell.insert({i,"cancelled"});
                cout<<"order with id no. "<<i<<" is cancelled."<<endl;
                break;
            }
            if(flag==1) break;
            temp=(temp->next);
        }while(temp!=NULL);
        }
        if(flag==1) break;
    }
    if(flag==0) cout<<"id not found"<<endl;
}

void SellCancel(int i){
    int flag=0;
    for(int j=0;j<ArraySellNo;j++){
        struct SellPricePoint* temp = new (struct SellPricePoint);
        temp=&ArraySell[j];
        if(temp->id==i && temp->next==NULL){
            SellShiftLeft(i);
            ArraySellNo--;
            flag=1;
            Buy.erase(i);
            Buy.insert({i,"cancelled"});
            cout<<"order with id no. "<<i<<" is cancelled."<<endl;
        }
        else{
        do{
            if(flag==1) break;
            if(temp->id==i){
                flag=1;
                Buy.erase(i);
                Buy.insert({i,"cancelled"});
                cout<<"order with id no. "<<i<<" is cancelled."<<endl;
                break;
            }
            if(flag==1) break;
            temp=(temp->next);
        }while(temp!=NULL);
        }
        if(flag==1) break;
    }
    if(flag==0) cout<<"id not found"<<endl;
}

int main()
{
    cout<<"Enter 1 to place a Buy Order \n2 to place a Sell Order \n3 to convert Limit Order to Market order \n4 to Cancel an Order"<<endl;
    int a=1;
    while(a!=0){
        cin>>a;
        if(a==1){
            cout<<"enter 1 to place Market Order, 2 for Limit Order"<<endl;
            int b,n;
            float p;
            cin>>b;
            cout<<"Enter number of orders"<<endl;
            cin>>n;
            if(b==1){
            PlaceMarketBuyOrder(n);
            }
            if(b==2){
            cout<<"Enter price of each order"<<endl;
            cin>>p;
            if(ArraySellNo>0 && p>=ArraySell[0].price) MatchLimitBuy(n,p);
            else PlaceLimitBuyOrder(n,p);
            }
        
        }
        else if(a==2){
        cout<<"Enter 1 to place Market Order, 2 for Limit Order"<<endl;
        int b,n;
        float p;
        cin>>b;
        cout<<"Enter number of orders"<<endl;
        cin>>n;
        if(b==1){
            PlaceMarketSellOrder(n);
        }
        if(b==2){
            cout<<"Enter price of each order"<<endl;
            cin>>p;
            if(ArrayBuyNo>0 && p<=ArrayBuy[0].price) {
                MatchLimitSell(n,p);
            }
            else {PlaceLimitSellOrder(n,p);}
        }
        }
        else if(a==3){
        cout<<"Enter the unique order id"<<endl;
        int i,t;
        cin>>i;
        cout<<"Enter 1 to place Market Order, 2 for Limit Order"<<endl;
        cin>>t;
        if(t==1) BuyLimitToMarket(i);
        else SellLimitToMarket(i);
        }
    
        else if(a==4){
        cout<<"enter the unique order id(i)"<<endl;
        int t,i;
        cin>>i;
        cout<<"Enter 1 to place Market Order, 2 for Limit Order"<<endl;
        cin>>t;
        if(t==1) BuyCancel(i);
        else SellCancel(i);
        }
    //cout<<ArrayBuy[0].price<<" "<<ArrayBuy[0].id<<endl;
    //cout<<ArraySell[0].price<<" "<<ArraySell[0].id<<endl;
    }
    map <int , string >:: iterator it;
    for( it=Buy.begin();it!=Buy.end();it++){
        cout<<it->first<<"  "<<it->second<<endl;
    }
    for( it=Sell.begin();it!=Sell.end();it++){
        cout<<it->first<<"  "<<it->second<<endl;
    }
    cout<<ArrayBuyNo<<"  "<<ArraySellNo<<endl;
    cout<<BuyId<<"  "<<SellId<<endl;
    return 0;
}





