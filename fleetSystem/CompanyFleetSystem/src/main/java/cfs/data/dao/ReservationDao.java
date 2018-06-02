package cfs.data.dao;

import cfs.data.entity.Car;
import cfs.data.entity.Reservation;
import java.time.LocalDate;
import java.util.List;
import org.springframework.data.repository.CrudRepository;

/**
 * Reservation data access object. The implementation of the interface is
 * generated automatically during the runtime.
 */

public interface ReservationDao extends CrudRepository<Reservation, Long> {
  public List<Reservation> findByCarModelNot(String model);

  public List<Reservation> findByCarAndState(Car car, String open);

  public List<Reservation> findByTripEnddateBeforeAndState(LocalDate now, String open);

  public List<Reservation> findByTripEnddateAfterAndTripStartdateBefore(LocalDate start_date, LocalDate end_date);

}
