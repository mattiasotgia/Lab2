{   
    std::cout << "File : " << std::flush;
    std::string filen;
    std::cin >> filen;
    std::ifstream data(filen.c_str());

    std::ofstream out(("new_" + filen).c_str());
    double q,w,e,r;
    double t;
    while(data >> q >> w >> e >> r >> t){
        out << q << " " << w << " " << e << " " << r << std::endl;
    }
}