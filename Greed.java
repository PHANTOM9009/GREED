import java.util.*;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
//174

class attribute {
    protected Entity entity;
    protected double cost;
    protected int ship_id;
    protected int cannon_id;

    protected void update(double c, int sid, int cid, int key) {
        if (key == 2369) {
            cost = c;
            ship_id = sid;
            cannon_id = cid;
        }
    }

    protected void updateShipStatus(int sid, int key) {
        if (key == 2369) {
            ship_id = sid;
        }
    }

    public attribute() {
        cost = -1;
        ship_id = -1;
        cannon_id = -1;
    }

    public attribute(Entity ent, double c, int sid, int cid) {
        cost = c;
        ship_id = sid;
        cannon_id = cid;
    }

    public double getCost() {
        return cost;
    }

    public int getShipId() {
        return ship_id;
    }

    public int getCannonId() {
        return cannon_id;
    }

    public Entity getEntity() {
        return entity;
    }
}

class Greed {

    public class cannon {
        public int getCannonId() {
            return cannon_id;
        }

        public double getCannonHealth() {
            // Add getCannonHealth implementation
            return health;
        }

        Greed.coords getCannonPosition() {
            // Add getCannonPosition implementation
            return tile;
        }

        public double getCannonMoney() {
            return money;
        }

        public boolean isCannonDead() {
            return isDead;
        }

        private int cannon_id;
        private double health;
        private Greed.coords tile;
        private Greed.abs_pos absolute_position;//abs pos of the middle of the cannon tile for the purpose of finding the slope.
        private double money;
        private boolean isDead;//0 for dead 1 for alive
        private int radius;
        private boolean busy;//to check if the cannon is currently busy
        private double current_angle;//current angle of the cannon
        private double req_angle;//angle required for the cannon
        private List<Integer> target_ships;//id of the ship that were targeted by the cannon, the last ship being the ship under attack by the cannon
        private int current_ship;//id of the current attacking ship
        private ShipSide ss;


        private void filter(List<Greed.abs_pos> ob);

        private void initializer(int cid, Greed.coords tile) {
            cannon_id = cid;
            this.tile = tile;
            absolute_position = Greed.abs_pos((int) tile.c * len / 2, (int) tile.r * len / 2);
        }

        private boolean isShipInMyRadius(int s_id, ShipSide opponent_side){
            if (s_id >= 0) {
                Greed.coords ob = tile;
                Control obb = new Control();
                ArrayList<ship> l1 = obb.getShipList(2369);

                Greed.coords position;
                if (opponent_side == ShipSide.FRONT) {
                    position = l1.get(s_id).getCurrenntTile_withoutMutex();
                } else if (opponent_side == ShipSide.REAR) {
                    position = l1.get(s_id).getCurrentRearTile_withoutMutex();
                }

                int c1 = position.c - radius;
                if (c1 < 0) {
                    c1 = 0;
                } else if (c1 > columns) {
                    c1 = columns - 1;
                }
                int c2 = position.c + radius;
                if (c2 < 0) {
                    c2 = 0;
                } else if (c2 > columns) {
                    c2 = columns - 1;
                }
                int r1 = position.r - radius;
                if (r1 < 0) {
                    r1 = 0;
                } else if (r1 > rows) {
                    r1 = rows - 1;
                }
                int r2 = position.r + radius;
                if (r2 < 0) {
                    r2 = 0;
                } else if (r2 > rows) {
                    r2 = rows - 1;
                }

                // checking if it's in the radius or not;
                if (ob.c >= c1 && ob.c <= c2 && ob.r >= r1 && ob.r <= r2) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }

        public cannon(){
            cannon_id = -1;
            health = 500;
            money = 1000;
            isDead = false;
            radius = 2;
            busy = false;
            current_angle = 0;
            current_ship = -1;
        }
    }

    public class abs_pos {
        public double x;
        public double y;

        public abs_pos() {
        }

        public abs_pos(double xx, double yy) {
            x = xx;
            y = yy;
        }
    }

    public class coords {
        public int r;
        public int c;
        public int cost;
        public double heuristic;
        public Entity tile_id;
        public int cannon_id;
        public Bonus bonus;

        public coords() {
            cost = 1;
            cannon_id = -1;
        }

        public coords(int rr, int cc, int co) {
            r = rr;
            c = cc;
            cost = co;
        }

        public coords(int rr, int cc, int co, Entity tile, int c_id, Bonus b) {
            r = rr;
            c = cc;
            cost = co;
            tile_id = tile;
            cannon_id = c_id;
            bonus = b;
        }

        public boolean equals(coords two) {
            return r == two.r && c == two.c;
        }

        public boolean notEquals(coords two) {
            return r != two.r || c != two.c;
        }
    }
}

 
class shipInfo {
    private ship ob;

    private Greed.coords getCurrentTile_withoutMutex(){
        return ob.getCurrenntTile_withoutMutex();
    }

    private Greed.coords getCurrentRearTile_withoutMutex(){
        return ob.getCurrentRearTile_withoutMutex();
    }

    public shipInfo(ship ob) {
        this.ob = ob;
    }

    public int getTotalShips() {
        return ob.getTotalShips();
    }

    public int getShipId() {
        return ob.getShipId();
    }
    public int getShipRadius(){
        return ob.getShipRadius();
    }
    public double getCurrentHealth(){
        return ob.getCurrentHealth();
    }
    public double getCurrentGold(){
     return ob.getCurrentGold();   
    }
    public double getCurrentAmmo();
    public double getCurrentResources();

    public double getCurrentFuel(){
        return ob.getCurrentFuel();
    }

    public int getDiedStatus(){
    return ob.getDiedStatus();

    }

    public Direction getShipDirection(){
    return ob.getShipDirection();
    }

    public Greed.coords getCurrentTile(){

    return ob.getCurrentTile();
    }

    public Greed.coords getCurrentRearTile(){
    return ob.getCurrentRearTile();
    }

    public Greed.abs_pos getRealAbsolutePosition(){
    return ob.getRealAbsolutePosition();
    }
}

    class Event {
        private static long count = 0;

        private long event_id;
        private int sid;
        private double ttl;

        
        public double timestamp;
        public EventType eventType;

        public Event() {
            eventType = EventType.NA;
        }

        public void setEventId() {
            event_id = count;
            count++;
        }

        public Event(double timestamp, int event_id, int sid, double ttl) {
            this.timestamp = timestamp;
            this.event_id = event_id;
            this.sid = sid;
            this.ttl = ttl;
            eventType = EventType.NA;
            count++;
        }

        public enum EventType {
            ShipsInMyRadius, IamInShipRadius, IamInCannonRadius, CannonsInMyRadius,
            ShipFire, CannonFire, LowHealth, NoAmmo, LowAmmo, NoFuel, LowFuel,
            ShipCollision, NA
        }

        public void initialize(double timestamp, EventType et, int sid) {
            this.timestamp = timestamp;
            this.sid = sid;
            eventType = et;
            ttl = timestamp;
        }

        public class ShipCollision {
            private Vector<Integer> sid;

            public ShipCollision() {
            }

            public ShipCollision(Vector<Integer> s) {
                sid = s;
            }

            public Vector<Integer> getShipId() {
                return sid;
            }
        }

        public class LowFuel {
            private double fuel;

            public LowFuel() {
            }

            public LowFuel(double f) {
                fuel = f;
            }

            public double getFuel() {
                return fuel;
            }
        }

        public class LowAmmo {
            private double ammo;

            public LowAmmo() {
            }

            public LowAmmo(double a) {
                ammo = a;
            }

            public double getAmmo() {
                return ammo;
            }
        }


        //abhi 315 line se thoda aage mai event class ka smaan aayega fir new class will start
        class LowHealth {
            private double health;

            public LowHealth() {
            }

            public LowHealth(double h) {
                health = h;
            }

            public double getHealth() {
                return health;
            }
        }

        class CannonFire {
            private Map<Integer, Greed.Bullet> cid;

            public CannonFire() {
                cid = new HashMap<>();
            }

            public CannonFire(Map<Integer, Greed.Bullet> c) {
                cid = c;
            }

            public Map<Integer, Greed.Bullet> getInfo() {
                return cid;
            }
        }

        class ShipFire {
            private Map<Integer, Greed.Bullet> ob;

            public ShipFire() {
                ob = new HashMap<>();
            }

            public ShipFire(Map<Integer, Greed.Bullet> s) {
                ob = s;
            }

            public Map<Integer, Greed.Bullet> getShipId() {
                return ob;
            }
        }

        class RadiusCannon {
            private Vector<Integer> cid;

            public RadiusCannon() {
                cid = new Vector<>();
            }

            public RadiusCannon(Vector<Integer> c) {
                cid = c;
            }

            public Vector<Integer> getCannonId() {
                return cid;
            }
        }

        class RadiusShip {
            private Vector<Integer> sid;

            public RadiusShip() {
                sid = new Vector<>();
            }

            public RadiusShip(Vector<Integer> s) {
                sid = s;
            }

            public Vector<Integer> getShipId() {
                return sid;
            }
        }

        RadiusShip radiusShip = new RadiusShip();
        RadiusCannon radiusCannon = new RadiusCannon();
        ShipFire shipFire = new ShipFire();
        CannonFire cannonFire = new CannonFire();
        LowHealth lowHealth = new LowHealth();
        LowAmmo lowAmmo = new LowAmmo();
        LowFuel lowFuel = new LowFuel();
        ShipCollision shipCollision = new ShipCollision();

    }

class upgrade_data {
    public int id;
    public int type;
    public int n;

    public upgrade_data() {
        id = -1;
    }

    public upgrade_data(int t, int n) {
        type = t;
        this.n = n;
        id = -1;
    }
}

class navigation {
    private int id;
    private int type;
    private Greed.coords target;
    private int s_id;
    private int n;
    private Direction dir;

    public navigation() {
        id = -1;
    }

    public navigation(int t, Greed.coords tar, int sid, int nn, Direction d) {
        id = -1;
        type = t;
        target = tar;
        s_id = sid;
        n = nn;
        dir = d;
    }
}

class bullet_data {
    public int bullet_id;
    public int type;
    public Greed.cannon can;
    public int s_id;
    public ShipSide s;
    public int c_id;

    public bullet_data() {
        bullet_id = -1;
    }

    public bullet_data(int type, Greed.cannon can, int s_id, ShipSide s, int c_id) {
        bullet_id = -1;
        this.type = type;
        this.can = can;
        this.s_id = s_id;
        this.s = s;
        this.c_id = c_id;
    }
}

class map_cost {
    public double new_cost;
    public Greed.coords ob;

    public map_cost() {
    }

    public map_cost(double n, Greed.coords o) {
        new_cost = n;
        ob = o;
    }
}

class shipData_exceptMe {
    private int ammo;
    private int died;
    private Direction dir;
    private int ship_id;
    private boolean isFiring;
    private int score;
    private double health;
    private double gold;
    private int motion;
    private int fuel;
    private int invisible;
    private Greed.coords tile_pos_front;
    private Greed.coords tile_pos_rear;
    private Greed.abs_pos front_abs_pos;
    private Greed.abs_pos rear_abs_pos;
    private Greed.abs_pos absolutePosition;

}

//control and ship class mai locks dekhne hai
class Control {
    protected final Lock m1 = new ReentrantLock(); // mutex lock for ship_list
    protected final Lock m2 = new ReentrantLock(); // mutex lock for cannon_list
    protected final Lock m3 = new ReentrantLock(); // mutex for bonus
    protected final Lock m4 = new ReentrantLock(); // mutex for storm
    protected static ArrayList<ship> ship_list;
    protected static List<Greed.cannon> cannon_list;
    protected static List<Greed.coords> bonus; // list of the coords having bonus
    protected static List<Greed.coords> storm; // list of the coords having storm right now
    protected static List<Greed.coords> opaque_coords;

    protected void setBonusList(List<Greed.coords> bonus) {
        try {
            m3.lock();
            this.bonus = bonus;
        } finally {
            m3.unlock();
        }
    }

    protected void setStormList(List<Greed.coords> storm) {
        try {
            m4.lock();
            this.storm = storm;
        } finally {
            m4.unlock();
        }
    }

    protected List<Greed.coords> getOpaqueCoords() {
        return opaque_coords;
    }

    protected List<ship> getShipList(int key) {
        if (key == 2369) {
            try {
                m1.lock();
                return ship_list;
            } finally {
                m1.unlock();
            }
        }
        return null; // You may want to handle other cases according to your logic
    }

    protected List<Greed.cannon> getCannonList(int key) {
        if (key == 2369) {
            try {
                m2.lock();
                return cannon_list;
            } finally {
                m2.unlock();
            }
        }
        return null; // You may want to handle other cases according to your logic
    }

    protected void setShipList(ArrayList<ship> ob, int key) {
        if (key == 2369) {
            try {
                m1.lock();
                ship_list = ob;
            } finally {
                m1.unlock();
            }
        }
    }

    protected List<Greed.coords> getBonusList() {
        try {
            m3.lock();
            return bonus;
        } finally {
            m3.unlock();
        }
    }

    protected List<Greed.coords> getStormList() {
        try {
            m4.lock();
            return storm;
        } finally {
            m4.unlock();
        }
    }
}

class ship {
    private Event events;
    
    private static int total;
    private int mutex_id;
    private Greed map_ob;
    private int ship_id;
    
    private ArrayList<Integer> collidedShips;
    private ArrayList<upgrade_data> udata;
    

    /*private long currentFrameNo;
    private double elapsed_time;
    private int frameRateLimit;*/
    //above code by chatgpt replace for chrono
    private long current_time;
    private long starting_time_limiter;


    private boolean hector;//to check if frame_rate_limiter is running for the first time or not
    private int current_frame_no;
    private double elapsed_time;
    private int frame_rate_limit;

    public Map<Integer, Integer> bullet_id_count;
    public Map<Integer, Integer> upgrade_id_count;
    public Map<Integer, ArrayList<map_cost>> upgrade_cost;
    public Map<Integer, Integer> cost_id_count;
    public Map<Integer, Integer> nav_id_count;

    public int navigation_count;

    public int collide_count;
    public int hit_bullet_count;
    public int cost_upgrade_count;
    public int bullet_count;
    public int client_fire;
    public int server_fire;
    public boolean gameOver;
    public boolean isFiring;




    public int isShipInMotion()//this function has to be used
    {
        //error. yha pr lock aayega
        return motion;
    }


    //error. ye implemet krne hai
    //public boolean frame_rate_limiter();//function to maintain the frame rate of the user function
    //public boolean getNextCurrentEvent(Event ob);

    public double threshold_health;
    public double threshold_ammo;
    public double threshold_fuel;



    public ArrayList<Event> current_event;//current events
    public ArrayList<Event> passive_event;//lingering events

    public ArrayList<Event> getPassiveEvent()
    {
        //error. lock aayega
        return passive_event;
    }//returns the passive event queue 
    public int passive_event_point;

    

    private int seconds;//seconds lived

    
    private int minutes;//minutes lived
    private int killer_ship_id;//the ship that killed you if that is the case
    private int killer_cannon_id;//the cannon that killed you if that is the case
    private ArrayList<Integer> killed_ships;//the list of all the ships killed by you..
    private int score;
    private String name;
    private int current_event_point;//pointer to current_event
    private ArrayList<navigation> nav_data;
    private ArrayList<navigation> nav_data_temp;//to store the nav_data temorarily
    private ArrayList<navigation> nav_data_final;//data that will be sent
    
    private static List<Greed.cannon> cannon_list;//cannon class is required
    private Greed.abs_pos front_abs_pos;//topmost coordinates of the tip of the ship:: will be updated in update_tile_pos
    private Greed.abs_pos rear_abs_pos;//endmost coordinates of the ship ==> will be updated in update_tile_pos
    //Map::abs_pos absolutePosition;//always stores the top left coordinate of the ship tile
    private Direction dir;
    //position is defined by the pointing part of the ship..look for it


    private int radius;//square radius
    private int bullet_radius;//radius of the bullet...initially this value is equal to the radius of the ship
    private double health; //to be changed
    private double gold;// to be changed
    private double resources;// to be changed
    private int died;//1 for dead, 0 for alive //to be changed
    private int motion; //1 for moving, 0 for stop
    private int fuel;//fuel is used for moving the ship around, once its over the ship cant move
    private int invisible;//to be changed
    private int ammo; // to be updated
    private int bullet_pointer;
    private int xbow; // to be updated
    private int shield;//if this is on, then no bullet can damage the ship...this is given for a limited time
    //data structure responsible for movement

    
    private Greed.coords tile_pos_front;
    private Greed.coords tile_pos_rear;

    
    private ArrayList<shipInfo> shipInfoList;



    
    // ... Rest of the members...

    // Constructors, getters, and other methods...

    private void updateFuel(int n) {
        //error. lock aayega
        fuel = n;
    }

    private Greed.abs_pos getAbsolutePosition1(ShipSide s) {
        if (s == ShipSide.FRONT)
            return front_abs_pos;
        else if (s == ShipSide.REAR)
            return rear_abs_pos;
        return null;
    }

    private Greed.abs_pos getAbsolutePosition1() {
        return front_abs_pos;
    }

    public Greed.abs_pos getRealAbsolutePosition() {
        //error. lock aayega
            return absolutePosition;
    }

    public Greed.abs_pos getAbsolutePosition(ShipSide s) {
            if (s == ShipSide.FRONT)
                return front_abs_pos;
            else if (s == ShipSide.REAR)
                return rear_abs_pos;
            return null;
    }
    public Greed.abs_pos getAbsolutePosition() {
            return front_abs_pos;
    }


    //error. ye krna hai abhi
    public void Greed_updateCost(Greed.coords ob, double new_cost) {
        // Implementation...
    }

    public ArrayList<Greed.coords> getRadiusCoords_ship(int s_id){
        auto cmp = [](Greed::coords a, Greed::coords b) {
        if (a.r == b.r)
        {
            return a.c < b.c;
        }
        if (a.c == b.c)
        {
            return a.r < b.r;
        }
        else
        {
            return a.r < b.r;
        }
        };
    Map<Greed.coords, int, decltype(cmp)> arr(cmp);
    Control obb;
    deque<ship*> l1;
    l1 = obb.getShipList(2369);

    int radius = l1[s_id]->getShipRadius();
    Greed::coords position;

    position = l1[s_id]->getCurrentTile();//first doing with the front position of the ship

    int c1 = position.c - radius;
    if (c1 < 0)
    {
        c1 = 0;
    }
    else if (c1 > columns)
    {
        c1 = columns - 1;
    }
    int c2 = position.c + radius;
    if (c1 < 0)
    {
        c1 = 0;
    }
    else if (c1 > columns)
    {
        c1 = columns - 1;
    }
    int r1 = position.r - radius;
    if (r1 < 0)
    {
        r1 = 0;
    }
    else if (r1 > rows)
    {
        r1 = rows - 1;
    }
    int r2 = position.r + radius;
    if (r2 < 0)
    {
        r2 = 0;
    }
    else if (r2 > rows)
    {
        r2 = rows - 1;
    }
    for (int i = r1; i <= r2; i++)
    {
        for (int j = c1; j <= c2; j++)
        {
            arr.insert(pair<Greed::coords, int>(Greed::coords(i, j), 1));
        }
    }
    //now doing the same stuff with rear coordinates

    position = l1[s_id]->getCurrentRearTile();

    c1 = position.c - radius;
    if (c1 < 0)
    {
        c1 = 0;
    }
    else if (c1 > columns)
    {
        c1 = columns - 1;
    }
    c2 = position.c + radius;
    if (c1 < 0)
    {
        c1 = 0;
    }
    else if (c1 > columns)
    {
        c1 = columns - 1;
    }
    r1 = position.r - radius;
    if (r1 < 0)
    {
        r1 = 0;
    }
    else if (r1 > rows)
    {
        r1 = rows - 1;
    }
    r2 = position.r + radius;
    if (r2 < 0)
    {
        r2 = 0;
    }
    else if (r2 > rows)
    {
        r2 = rows - 1;
    }
    for (int i = r1; i <= r2; i++)
    {
        for (int j = c1; j <= c2; j++)
        {
            arr.insert(pair<Greed::coords, int>(Greed::coords(i, j), 1));
        }
    }
    vector<Greed::coords> l;
    for (auto c : arr)
    {
        l.push_back(c.first);

    }
    return l;
    }//function to return the tiles that are in the radius of a particular entity, just pass the id of the ship
    public ArrayList<Greed.bullet> getBulletHitList()
    {
        //error. lock aayega
        return bullet_hit;
    }
    public int getTotalBulletsHit()
    {
        //error. lock aayega
        return bullet_hit.howMany();
    }
    /*
    int getTotalBulletsFired()
    {
        unique_lock<mutex> lk(mutx->m[ship_id]);
        return cannon_ob.allBullets.howMany();
    }
    */

    public Greed.bullet getLastHitBullet()//returns the last bullet that had hit the ship
    {
        //error. lock aayega
        return bullet_hit.get(bullet_hit.howMany() - 1);
    }
    public Greed.bullet getLastBulletFired()//returns the last fired bullet
    {
        //error. lock aayega
        return cannon_ob.activeBullets.get(cannon_ob.activeBullets.size() - 1);
    }
    public ArrayList<Greed.bullet> getHitBulletList()//list of the bullets that had hit the ship
    {
        //error. lock aayega
        ArrayList<Greed.bullet> ret;
        for (int i = 0; i < bullet_hit.howMany(); i++)
        {
            ret.add(bullet_hit.get(i));
        }
        return ret;
    }
    
    //this class will have the API for  the user, some functions will be inaccessible to the user, inaccessible functions will be protected by an id
    ///functions for returning things.
    public ArrayList<shipInfo> getShipList()
    {
        return shipInfoList;
    }

    public int getShipRadius()
    {
        //unique_lock<mutex> lk(mutx->m[mutex_id]);
        return radius;
    }
    public double getCurrentFuel()
    {
        //error. lock aayega
        return fuel;
    }
    public Direction getShipDirection()
    {
        //error. lock aayega
        return dir;
    }
    public int getShipId()
    {
        return ship_id;
    }
    public Greed.coords getCurrentTile()//returns the front tile
    {
        //error. lock aayega
        return tile_pos_front;

    }
    public Greed.coords getCurrenntTile_withoutMutex()
    {
        return tile_pos_front;
    }
    public Greed.coords getCurrentRearTile_withoutMutex()
    {
        return tile_pos_rear;
    }
    public Greed.coords getCurrentRearTile()//returns the rear tile of the ship
    {
        //error. lock aayega
        return tile_pos_rear;

    }

    public double getcurrentRadius()
    {
        //error. lock aayega
        return radius;
    }
    public double getCurrentHealth()
    {
        //error. lock aayega
        return health;
    }

    public int getDiedStatus()
    {
        //error. lock aayega
        return died;
    }

    public int getCurrentAmmo()
    {

        //error. lock aayega
        return ammo;
    }

    public int getTotalShips()
    {
        return total;
    }
    public double getCurrentGold()
    {
        //error. lock aayega
        return gold;
    }
    //functions for updating some values of the game

    //starting of the API
    attribute whatsHere(Greed.coords ob, int m = 0);//leave this for now
    // List<attribute> whatsHere(Map::abs_pos ob);

    public ArrayList<bullet_data> bullet_info;
    public int count_upgrade;//number of times the agent has sent the upgrading request


    public ArrayList<Greed.cannon> getCannonList()
    {
        ArrayList<Greed.cannon> ret = new ArrayList<>();
        for (int i = 0; i < cannon_list.howMany(); i++)
        {
            ret.add(cannon_list.get(i));
        }
        return ret;
    }
    
    public int getIndex(List<Greed.coords> ob, Greed.coords val)
    {
        int index = -1;
        for (int i = 0; i < ob.howMany(); i++)
        {
            if (val.c == ob.get(i).c && val.r == ob.get(i).r)
            {
                index = i;
                return index;
            }
        }
        return index;

    }
    public boolean isShipMoving(int s_id)// tells if another player's ship is moving
    {
        ship ob = shipInfoList.get(s_id).ob; //assuming that no ship entry is deleted and every ship is in increasing order of their ship_id
        if (ob.motion == 1)
        {
            return true;
        }
        else if (ob.motion == 0)
        {
            return false;
        }else {
            return false;
        }
    }
    public boolean isShipMoving()
    {
        //return isShipMoving(this->ship_id);
         //current mechanism is to check if the list of absolute coords is >0
        //error. lock aayega
        return motion;

    }
    
    //introducing the new functions
    public void Greed_sail(Direction d, int tiles)
    {

        //error. lock aayega
        if (motion == 0)
        {
            navigation nav(1, new Greed.coords(-1, -1), -1, tiles, d);
            nav_data.add(nav);
        }
    }
    public void Greed_sail(Direction d)
    {

        //error. lock aayega

        int tiles = 1;
        if (motion == 0)
        {
            navigation nav(1, new Greed.coords(-1, -1), -1, tiles, d);
            nav_data.add(nav);
        }
    }
    public void Greed_setPath(int s_id)
    {

        //error. lock aayega
        if (motion == 0)
        {
            navigation nav(0, new Greed.coords(-1, -1), s_id, -1, Direction::NA);
            nav_data.add(nav);
        }
    }
    public void Greed_setPath(Greed.coords ob)
    {
        //error. lock aayega
        if (motion == 0)
        {
            navigation nav(0, ob, -1, -1, Direction.NA);
            nav_data.add(nav);
        }
    }
    public void Greed_chaseShip(int s_id)
    {

        //error. lock aayega
        if (motion == 0)
        {
            navigation nav(2, new Greed.coords(-1, -1), s_id, -1, Direction.NA);
            nav_data.add(nav);
            lock_chase_ship = 1;
        }

    }
    public void Greed_anchorShip()
    {
        //error. lock aayega
        if (motion == 1)
        {
            navigation nav(3, Greed.coords(-1, -1), -1, -1, Direction.NA);
            nav_data.add(nav);
        }
    }
    //entity conversion functions
    public void Greed_fireCannon(cannon can, int s_id, ShipSide s)
    {
        //error. lock aayega

        bullet_info.add(new bullet_data(0, can, s_id, s, -1));

    }


    public void Greed_fireAtCannon(int c_id, Greed.cannon can)
    {
        //error. lock aayega

        bullet_info.add(new bullet_data(1, can, -1, ShipSide.NA, c_id));
    }

    //error. yha overload lgana hai
    public void Greed_fireAtCannon(int c_id)
    {
        //error. lock aayega
        Greed.cannon can = new cannon();
        bullet_info.add(new bullet_data(1, can, -1, ShipSide.NA, c_id));
    }
    public ArrayList<upgrade_keeping> upgrade_queue;
    public void Greed_upgradeHealth(int n)
    {
        //error. lock aayega
        if (lock_health == 0)
        {

            udata.add(upgrade_data(1, n));
            lock_health = 1;


        }
    }
    public void Greed_upgradeAmmo(int n)
    {
        //error. lock aayega
        if (lock_ammo == 0)
        {

            udata.add(upgrade_data(0, n));
            lock_ammo = 1;

        }
    }
    public void Greed_upgradeFuel(int n)
    {
        //error. lock aayega
        if (lock_fuel == 0)
        {

            udata.add(upgrade_data(2, n));
            lock_fuel = 1;

        }
    }


    
    //dekho yha se maine apna krna shuru kr diya hai
    public ArrayList<Greed.coords> getRadiusCoords_cannon(int c_id) {
        if (cannon_list.howMany() > c_id && c_id >= 0) {
            List<Greed.coords> arr = new ArrayList<>();
            int radius = 2;
            Greed.coords position = cannon_list.get(c_id).getCannonPosition();

            int c1 = Math.max(position.c - radius, 0);
            if (c1 > columns)
        {
            c1 = columns - 1;
        }
            int c2 = Math.min(position.c + radius, columns - 1);
            if(c2<0) c2 = 0;
            int r1 = Math.max(position.r - radius, 0);
            if(r1>rows) r1 = rows - 1;
            int r2 = Math.min(position.r + radius, rows - 1);
            if(r2<0) r2 = 0;
            for (int i = r1; i <= r2; i++) {
                for (int j = c1; j <= c2; j++) {
                    arr.add(new Greed.coords(i, j));
                }
            }
            return arr;
        }
        return new ArrayList<>(); // Return an empty list if conditions are not met
    }
    public int lock_ammo;
    public int lock_chase_ship;
    public int lock_health;
    public int lock_fuel;
    //ye jo hai constructor chatGpt se nhi kiya hai
    public ship(){
        this.bullet_info = new ArrayList<>();
        this.collide_count = 0;
        this.hit_bullet_count = 0;
        this.navigation_count = 0;
        this.cost_upgrade_count = 0;
        this.count_upgrade = 0;
        bullet_count = 0;
        client_fire = 0;
        server_fire = 0;
        gameOver = false;
        hector = false;
        lock_ammo = 0;
        lock_fuel = 0;
        lock_health = 0;
        lock_chase_ship = 0;

        frame_rate_limit = 30;
        elapsed_time = 0;
        current_frame_no = -1;
        current_time = chrono::high_resolution_clock::time_point();

        isFiring = false;
        health = 200;
        motion = 0;

        //error. ye htaya hai
        //autopilot = 0;
        //pointPath = -1;
        //pointTilePath = 0;

        //making the localMap here
        died = 0;
        tile_pos_front = new Greed.coords(5, 5);
        dir = Direction.NORTH;
        fuel = DEF_FUEL;
        ammo = DEF_AMMO;
        radius = 2;
        bullet_radius = radius;
        current_event_point = 0;
        bullet_pointer = -1;
        shield = 0;
        gold = INITIAL_GOLD;
        killer_ship_id = -1;
        killer_cannon_id = -1;
        threshold_health = 10;
        threshold_ammo = 10;
        threshold_fuel = 10;
        score = 0;
        minutes = Integer.MAX_VALUE;
        seconds = Integer.MAX_VALUE;
    }

    public ArrayList<Integer> cannonsIamInRadiusOf()
    {
        Control ob;
        ArrayList<Integer> l = new ArrayList<>();// yha pe l new initialize kiya hai(assumed)
        List<Greed.cannon> cannon_list = ob.getCannonList(2369);
        for (int i = 0; i < cannon_list.howMany(); i++)
        {
            if (cannon_list.get(i).isDead == true)
            {
                continue;
            }
            //checking first for the front side of the ship
            boolean front = cannon_list.get(i).isShipInMyRadius(this.ship_id, ShipSide.FRONT);
            boolean rear = cannon_list.get(i).isShipInMyRadius(this.ship_id, ShipSide.REAR);
            if (front || rear)
            {
                l.add(cannon_list.get(i).cannon_id);
            }
        }
        return l;
    }

    public ArrayList<Integer> shipsInMyRadius()//returns the list of ships currently in my radius
    {
        //which side the user wants to check
        //here front and rear are of the calling ship
        ArrayList<Integer> l = new ArrayList<>();//the list to be returned
        for (int i = 0; i < shipInfoList.size(); i++)
        {
            if (shipInfoList.get(i).getShipId() != this.ship_id && shipInfoList.get(i).ob.died == 0)
            {
                boolean front = isShipInMyRadius(shipInfoList.get(i).getShipId(), ShipSide.FRONT);
                boolean rear = isShipInMyRadius(shipInfoList.get(i).getShipId(), ShipSide.REAR);
                if (front || rear)
                {
                    l.add(shipInfoList.get(i).getShipId());
                }

            }

        }
        return l;
    }

    public ArrayList<Integer> shipsIamInRadiusOf()
    {
        ArrayList<Integer> l = new ArrayList<>();
        for (int i = 0; i < shipInfoList.size(); i++)
        {
            if (shipInfoList.get(i).getShipId() != this.ship_id && shipInfoList.get(i).ob.died == 0)
            {
                boolean front, rear;//opponent's
                //for FRONT side
                front = isInShipRadius(shipInfoList.get(i).getShipId(), getCurrentTile(), ShipSide.FRONT);
                rear = isInShipRadius(shipInfoList.get(i).getShipId(), getCurrentTile(), ShipSide.REAR);
                boolean Front = false;
                boolean Rear = false;
                if (front || rear)
                {

                    Front = true;
                }

                //for Rear
                front = isInShipRadius(shipInfoList.get(i).getShipId(), getCurrentRearTile(), ShipSide.FRONT);
                rear = isInShipRadius(shipInfoList.get(i).getShipId(), getCurrentRearTile(), ShipSide.REAR);
                if (front || rear)
                {
                    Rear = true;
                }
                if (Front || Rear)
                {
                    l.add(shipInfoList[i].getShipId());
                }
            }
        }
        return l;
    }

    public double getDistance(int s_id)//returns the eucledian distance between two ships using the tile_pos
    {
        //just like finding the heuristic this is not the exact distance:
        //this is just an approximation of the original distance, original distance might vary
        Greed.coords c1 = shipInfoList.get(s_id).getCurrentTile();//getting the position of the ship in question
        Greed.coords c2 = getCurrentTile();
        return Math.sqrt(Math.pow((c1.r - c2.r), 2) + Math.pow((c1.c - c2.c), 2));

    }

    public attribute whatsHere(Greed.coords ob, int m)//pass the row and column of the tile to get whats there
    {
        /*
         * if m is 0 then global map is looked into for the attribute information
         * if m is 1 then local map is looked into for attribute information
         */
        // cout << "\n inside the function==>" << localGreed[10 * columns + 0].b.cost;
        int index = ob.r * columns + ob.c;//this is the index of ob in the graph
        List<Greed.vertex> graph;
        if (m == 0)
        {
            graph = getGlobalMapref(2369);
        }
        else if (m == 1)//call the localMap
        {
            //
            graph = localMap;

        }
        attribute att;
        Control cont;

        List<Greed.cannon> cannon = cont.getCannonList(2369);
        ArrayList<ship> sh = cont.getShipList(2369);
        att.entity = graph.get(index).b.tile_id;
        //checking for the water first if its a water then checking for the storm
        if (graph.get(index).b.tile_id == Entity.WATER)
        {


            att.cost = WATER_COST;


        }
        //checking if its land then it can be simple land,cannon, or bonus
        else if (graph.get(index).b.tile_id == Entity.LAND)
        {

            if (aux1(cannon, ob) != -1)
            {


                att.cannon_id = aux1(cannon, ob);
                att.cost = -1;

            }
            else
            {

                att.cost = -1;
            }
        }
        //checking for the ship
        int ind = -1;
        for (int i = 0; i < sh.size(); i++)
        {
            if (sh.get(i).tile_pos_front == ob)
            {
                ind = i;
                break;
            }
        }
        if (ind != -1)
        {

            att.ship_id = ind;
        }
        if (m == 1)
        {
            att.cost = graph.get(index).b.cost;
        }
        return att;

    }




    public boolean isInShipRadius(int s_id, Greed.coords ob, ShipSide opponent_side)//s_id is the id of the id whose radius has to be checked
    {

        Control obb;
        ArrayList<ship> l1;
        l1 = obb.getShipList(2369);
        if (l1.get(s_id).died == 1)
        {
            return false;
        }
        int radius = l1.get(s_id).getShipRadius();
        Greed.coords position;
        if (opponent_side == ShipSide.FRONT)
        {
            position = l1.get(s_id).getCurrenntTile_withoutMutex();
        }
        else if (opponent_side == ShipSide.REAR)
            position = l1.get(s_id).getCurrentRearTile_withoutMutex();

        int c1 = position.c - radius;
        if (c1 < 0)
        {
            c1 = 0;
        }
        else if (c1 > columns)
        {
            c1 = columns - 1;
        }
        int c2 = position.c + radius;
        if (c2 < 0)
        {
            c2 = 0;
        }
        else if (c2 > columns)
        {
            c2 = columns - 1;
        }
        int r1 = position.r - radius;
        if (r1 < 0)
        {
            r1 = 0;
        }
        else if (r1 > rows)
        {
            r1 = rows - 1;
        }
        int r2 = position.r + radius;
        if (r2 < 0)
        {
            r2 = 0;
        }
        else if (r2 > rows)
        {
            r2 = rows - 1;
        }
        //checking if its in the radius or not;
        if (ob.c >= c1 && ob.c <= c2 && ob.r >= r1 && ob.r <= r2)
        {
            return true;
        }
        else
            return false;
    }
    public boolean isInShipRadius(int s_id, ShipSide myside, ShipSide opponent_side)
    {
        if (shipInfoList.get(s_id).getDiedStatus() == 1)
        {
            return false;
        }
        for (int i = 0; i < shipInfoList.size(); i++)
        {
            if (shipInfoList.get(i).getShipId() != this.ship_id)
            {
                if (myside == ShipSide.FRONT)
                {
                    return isInShipRadius(s_id, getCurrentTile(), opponent_side);
                }
                else if (myside == ShipSide.REAR)
                {
                    return isInShipRadius(s_id, getCurrentRearTile(), opponent_side);
                }
            }
        }
        //yha end mai true false mai se kuch return krwana pdega
        tbhi error hai
    }
    public boolean isCannonInRadius(int c_id, ShipSide side)
    {
        if (cannon_list.get(c_id).isDead == true)
        {
            return false;
        }
        return isInShipRadius(this.ship_id, cannon_list.get(c_id).getCannonTile(), side);
    }
    public ArrayList<Integer> cannonsInMyRadius()
    {

        Control ob;
        List<Greed.cannon> cannon_list = ob.getCannonList(2369);
        ArrayList<Integer> l = new ArrayList<>();
        for (int i = 0; i < cannon_list.howMany(); i++)
        {
            if (cannon_list.get(i).isDead == true)
            {
                continue;
            }
            boolean front = false;
            boolean rear = false;
            front = isCannonInRadius(cannon_list.get(i).cannon_id, ShipSide.FRONT);
            rear = isCannonInRadius(cannon_list.get(i).cannon_id, ShipSide.REAR);
            if (front || rear)
            {
                l.add(cannon_list.get(i).cannon_id);

            }
        }
        return l;

    }
    public boolean isShipInMyRadius(int s_id, ShipSide oppSide)//to check if s_id is in the callers ship radius
    {
        //this works as follows
        /*
         * this checks if oppSide(side of the opponent) is in the radius of current ship irrespective of its cannon position(both for front and rear)
         */
        Control ob;
        ArrayList<ship> l;
        l = ob.getShipList(2369);
        if (s_id <= l.size() - 1 && s_id >= 0 && l.get(s_id).died == 0)
        {
            Greed.coords pos;
            //chekcing for front side of opponent first
            if (oppSide == ShipSide.FRONT)
            {
                Greed.coords pos = l.get(s_id).getCurrenntTile_withoutMutex();
                boolean check = isInShipRadius(this.ship_id, pos, ShipSide.FRONT);
                boolean check1 = isInShipRadius(this.ship_id, pos, ShipSide.REAR);
                if (check || check1)
                {
                    return true;
                }
                else
                    return false;
            }
            else if (oppSide == ShipSide.REAR)
            {
                Greed.coords pos = l.get(s_id).getCurrentRearTile_withoutMutex();
                boolean check = isInShipRadius(this.ship_id, pos, ShipSide.FRONT);
                boolean check1 = isInShipRadius(this.ship_id, pos, ShipSide.REAR);
                if (check || check1)
                {
                    return true;
                }
                else
                    return false;
            }



        }
        //yha pr true false kuch return krna pdega
        tbhi ye error thik hoga
    }


}



class cannon_data
{
    public int cid;
    public double angle;
    public int picture;//which picture to display
    public double cannon_health;
    public int is_cannon_dead;
    public cannon_data(int c, double a, int pic, double h, int dead)
    {
        cid = c;
        angle = a;
        picture = pic;
        cannon_health = h;
        is_cannon_dead = dead;

    }
}