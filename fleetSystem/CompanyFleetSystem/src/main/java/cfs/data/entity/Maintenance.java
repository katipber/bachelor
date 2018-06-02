package cfs.data.entity;

import java.time.LocalDate;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;

/**
 * Maintenance entity class
 */

@Entity
public class Maintenance {
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  private Long    id;
  private LocalDate    in_date;
  private LocalDate    out_date;
  
  @ManyToOne
  private Car car;
  
  @Override
  public int hashCode() {
    return id.hashCode();
  }
  
  @Override
  public boolean equals(Object obj) {
    if(obj == null) return false;
    if(getClass() == obj.getClass())
      return hashCode() == obj.hashCode();
    return false;
  }

  public Maintenance() {
  }

  public Long getId() {
    return id;
  }

  public void setId(Long id) {
    this.id = id;
  }

  public LocalDate getIn_date() {
    return in_date;
  }

  public void setIn_date(LocalDate in_date) {
    this.in_date = in_date;
  }

  public LocalDate getOut_date() {
    return out_date;
  }

  public void setOut_date(LocalDate out_date) {
    this.out_date = out_date;
  }

  public Car getCar() {
    return car;
  }

  public void setCar(Car car) {
    this.car = car;
  }
  
}
